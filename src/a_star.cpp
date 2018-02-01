#include "heuristiques.hpp"
#include "a_star.hpp"
#include "donnees.hpp"
#include "tas.hpp"

#include <cstdint>
#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace std;

A_star::A_star(Donnees *d) {
	this->d = d;
	resolu = false;
	bestObj = -1;
	bestSol = NULL;
	N = d->getN();
	M = d->getM();
}

A_star::~A_star() {}

int A_star::plusPetitDomaine(noeud *n) {
	if (n->prof > 0) {
		uint_fast8_t prof = n->prof;
		uint_fast8_t ppDom = M+1;
		uint_fast8_t *etat = n->etat;
		int_fast8_t *indP = n->indP;
		bool realisable = true;

		int bestRes = -1;

		uint_fast8_t nj = 0;
		while (realisable && (nj < N)) {
			if (indP[nj] == -1) {
				uint_fast8_t cpt = 0;
				for (int m = 0; m < M; ++m) {
					uint_fast8_t i = 0;//cout << "COCO" << endl;
					while ((i < N) && ((indP[i] == -1) || !d->sontEnConflit(i,nj,etat[indP[i]],m))) {
						++i;
					}//cout << "COCO 2" << endl;
					if (i == N) {
						++cpt;
					}
				}
				if (cpt == 0) {
					bool realisable = false;
					bestRes = -1;
				} else if (cpt < ppDom) {
					bestRes = nj;
					ppDom = cpt;
				}
			}
			++nj;
		}

		return bestRes;
	} else {
		return 0;
	}
}

void A_star::resoudre() {
	cout << "Résolution..." << endl;
	manoeuvre **mans = d->getManoeuvres();
	uint_fast8_t *indMS = d->getIndMS();

	bestSol = (uint_fast8_t *) malloc(sizeof(uint_fast8_t));
	bestObj = N*mans[indMS[M-1]]->cout;			// Le meilleur objectif prend la pire valeur possible (N*max(cout(m)))
	Tas *tas = new Tas(N*M);

	Heuristiques *h = new Heuristiques(d);

	// INITIALISATION
	noeud *node;
	node = (noeud *) malloc(sizeof(noeud));
	node->prof = 0;
	node->coutActuel = 0;
	node->etat = NULL;
	node->indP = (int_fast8_t *) malloc(N*sizeof(int_fast8_t));
	for (int i = 0; i < N; ++i) {
		node->indP[i] = -1;
	}
	node->borneInf = bestObj;
	tas->empiler(node);

	//bestObj = 16;

	// ON PARCOURT L'ARBRE DE RECHERCHE
	do {
		// on récupère le noeud le plus prometteur
		node = tas->top();
		tas->depiler();
		if (node->prof < N) {
			// on crée tous ses fils (réalisables)
			int ppDom = plusPetitDomaine(node);
		//ppDom = node->prof;
			if (ppDom != -1) {
				for (int m = 0; m < M; ++m) {
					bool flag = false;
					int n = 0;
					while ((n < N) && (!flag)) {
						flag = (node->indP[n] != -1) && d->sontEnConflit(n, ppDom, node->etat[node->indP[n]], m);
						++n;
					}
					if (!flag) {
						int nouvLB = h->borneInfNaturelle(node, ppDom, m);
						if ((nouvLB != -1) && (nouvLB < bestObj)) {
							noeud *nouveau = (noeud *) malloc(sizeof(noeud));
							nouveau->coutActuel = node->coutActuel + mans[m]->cout;
							nouveau->prof = node->prof + 1;
							nouveau->borneInf = nouvLB;
							nouveau->selection = nouvLB;
							nouveau->etat = (uint_fast8_t *) malloc(nouveau->prof*sizeof(uint_fast8_t));
							nouveau->indP = (int_fast8_t *) malloc(N*sizeof(int_fast8_t));
							for (int k = 0; k < node->prof; ++k) {
								nouveau->etat[k] = node->etat[k];
								nouveau->indP[k] = node->indP[k];
							}
							nouveau->etat[node->prof] = m;
							for (int k = node->prof; k < N; ++k) {
								nouveau->indP[k] = -1;
							}
							nouveau->indP[ppDom] = node->prof;
							tas->empiler(nouveau);
						}
					}
				}
			}
			free(node->etat);
			free(node->indP);
			free(node);
		} else if (node->coutActuel < bestObj) {
			bestObj = node->coutActuel;
			free(bestSol);
			for (int i = 0; i < N; ++i) {
				bestSol[node->indP[i]] = node->etat[i];
			}
			free(node->indP);
			free(node);
			resolu = true;
		} else {
			free(node->etat);
			free(node->indP);
			free(node);
		}
	} while ((tas->getN() > 0) && (tas->top()->borneInf < bestObj));

	cout << "solution : " << bestObj << " ";
	for (int i = 0; i < N; ++i) {
		cout << (int) bestSol[i] << " ";
	}
	cout << endl;
}

uint_fast8_t *A_star::getSol() {
	assert(resolu);

	return NULL;
}

int A_star::getObj() {
	assert(resolu);

	return bestObj;
}