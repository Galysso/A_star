#include "heuristiques.hpp"
#include "a_star.hpp"
#include "donnees.hpp"
#include "tas.hpp"

#include <ctime>
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
	int_fast8_t *indP = n->indP;
	uint_fast8_t *etat = n->etat;

	int ppDom = -1;
	uint_fast8_t minSize = M+1;

	int_fast8_t indNj = 0;

	bool realisable = true;

	while ((indNj < N) && realisable) {
		int_fast8_t nj = indP[indNj];
		if (nj == -1) {
			uint_fast8_t cpt = 0;
			uint_fast8_t mj = 0;
			while ((mj < M) && realisable) {
				int_fast8_t indNi = 0;
				while ((indNi < N) && ((indP[indNi] == -1) || !d->sontEnConflit(indNi, indNj, etat[indP[indNi]], mj))) {
					++indNi;
				}
				if (indNi == N) {
					++cpt;
				}
				++mj;
			}
			if (cpt == 0) {
				realisable = false;
				ppDom = -1;
				minSize = 0;
				cout << "HAPPENED !" << endl;
			} else if (cpt < minSize) {
				ppDom = indNj;
				minSize = cpt;
			}
		}
		++indNj;
	}

	return ppDom;
}

void A_star::resoudre() {
	manoeuvre **mans = d->getManoeuvres();
	uint_fast8_t *indMS = d->getIndMS();

	bestSol = (uint_fast8_t *) malloc(sizeof(uint_fast8_t));
	bestObj = N*mans[indMS[M-1]]->cout + 1;			// Le meilleur objectif prend la pire valeur possible (N*max(cout(m)))
	Tas *tas = new Tas(N*M, bestObj);

	Heuristiques *h = new Heuristiques(d);

	cout << "Résolution..." << endl;
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
	node->borneInf = 0;
	node->selection = 0;
	tas->empiler(node);

	//bestObj = 11;
	long long niter = 0;
	// ON PARCOURT L'ARBRE DE RECHERCHE
	time_t debut;
	debut = clock();
	int nAffich = 1;
	do {
		++niter;
		// on récupère le noeud le plus prometteur
		node = tas->top();
		tas->depiler();
		if (node->prof < N) {
			//if (node->borneInf < bestObj) {
				// on crée tous ses fils (réalisables)
				int ppDom = plusPetitDomaine(node);
				//int ppDom = node->prof;
				if (ppDom != -1) {
					for (int m = 0; m < M; ++m) {
						// On vérifie que la manoeuvre est réalisable dans l'état courant
						int n = 0;
						while ((n < N) && ((node->indP[n] == -1) || !d->sontEnConflit(n, ppDom, node->etat[node->indP[n]], m))) {
							++n;
						}
						if (n == N) {
							int nouvLB = h->borneInfNaturelle(node, ppDom, m);
							//cout << "nouvLB = " << nouvLB << endl;
							if ((nouvLB != -1) && (nouvLB < bestObj)) {
								noeud *nouveau = (noeud *) malloc(sizeof(noeud));
								nouveau->coutActuel = node->coutActuel + mans[m]->cout;
								nouveau->prof = node->prof + 1;
								nouveau->borneInf = nouvLB;
								nouveau->etat = (uint_fast8_t *) malloc(nouveau->prof*sizeof(uint_fast8_t));
								nouveau->indP = (int_fast8_t *) malloc(N*sizeof(int_fast8_t));
								for (int k = 0; k < node->prof; ++k) {
									nouveau->etat[k] = node->etat[k];
									nouveau->indP[k] = node->indP[k];
								}
								nouveau->etat[node->prof] = m;
								for (int k = node->prof; k < N; ++k) {
									nouveau->indP[k] = node->indP[k];
								}
								nouveau->indP[ppDom] = node->prof;
								nouveau->selection = nouvLB;
								int UB;
								uint_fast8_t *Usol = h->completionGloutonne(node, &UB);
								if (UB != -1) {
									if (UB < bestObj) {
										bestObj = UB;
										free(bestSol);
										bestSol = Usol;
										cout << "(heuristique) nouvelle meilleure solution : (" << bestObj << ") ";
										for (int i = 0; i < N; ++i) {
											cout << (int) bestSol[i] << " ";
										}
										cout << endl << endl;
									}
									nouveau->selection = nouvLB;
								}
								tas->empiler(nouveau);
							}
						}
					}
				} else {
					cout << "ppDom == -1" << endl;
				}
			//}
			free(node->etat);
			free(node->indP);
			free(node);
		} else if (node->coutActuel < bestObj) {
			bestObj = node->coutActuel;
			free(bestSol);
			for (int i = 0; i < N; ++i) {
				bestSol[i] = node->etat[node->indP[i]];
			}
			free(node->indP);
			free(node);
			resolu = true;
			tas->setUB(bestObj);

			cout << "(exact) nouvelle meilleure solution : " << bestObj << " ";
			for (int i = 0; i < N; ++i) {
				cout << (int) bestSol[i] << " ";
			}
			cout << endl << endl;
		} else {
			free(node->etat);
			free(node->indP);
			free(node);
		}
		if (((double)(clock()-debut)/CLOCKS_PER_SEC) > nAffich) {
			cout << "nextLB=" << tas->top()->borneInf << endl;
			cout << "size(tas)=" << tas->getN() << endl;
			++nAffich;
		}
	} while ((tas->getN() > 0) && (tas->top()->borneInf < bestObj));

	cout << "solution : (" << bestObj << ") ";
	for (int i = 0; i < N; ++i) {
		cout << (int) bestSol[i] << " ";
	}
	cout << endl << endl;
}

uint_fast8_t *A_star::getSol() {
	assert(resolu);

	return NULL;
}

int A_star::getObj() {
	assert(resolu);

	return bestObj;
}