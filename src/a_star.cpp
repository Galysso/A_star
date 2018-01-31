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
}

A_star::~A_star() {}

void A_star::resoudre() {
	cout << "Résolution..." << endl;
	int N = d->getN();
	int M = d->getM();
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
	node->borneInf = bestObj;
	tas->empiler(node);

	//bestObj = 19;

	// ON PARCOURT L'ARBRE DE RECHERCHE
	do {
		// on récupère le noeud le plus prometteur
		node = tas->top();
		tas->depiler();
		if (node->prof < N) {
			// on crée tous ses fils (réalisables)
			for (int m = 0; m < M; ++m) {
				bool flag = false;
				int n = 0;
				while ((n < node->prof) && (!flag)) {
					flag = d->sontEnConflit(n, node->prof, node->etat[n], m);
					++n;
				}
				if (!flag) {
					int nouvLB = h->borneInfNaturelle(node->etat, node->prof, m, node->coutActuel);
					if ((nouvLB != -1) && (nouvLB < bestObj)) {
						noeud *nouveau = (noeud *) malloc(sizeof(noeud));
						nouveau->coutActuel = node->coutActuel + mans[m]->cout;
						nouveau->prof = node->prof + 1;
						nouveau->borneInf = nouvLB;
						nouveau->etat = (uint_fast8_t *) malloc(nouveau->prof*sizeof(uint_fast8_t));
						for (int k = 0; k < node->prof; ++k) {
							nouveau->etat[k] = node->etat[k];
						}
						nouveau->etat[node->prof] = m;
						tas->empiler(nouveau);
					}
				}
			}
			free(node->etat);
			free(node);
		} else if (node->coutActuel < bestObj) {
			bestObj = node->coutActuel;
			free(bestSol);
			bestSol = node->etat;
			free(node);
			resolu = true;
		} else {
			free(node->etat);
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