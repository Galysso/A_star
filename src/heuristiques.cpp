#include "heuristiques.hpp"
#include "donnees.hpp"
#include "a_star.hpp"

#include <iostream>
#include <cstdlib>
#include <cstdint>

using namespace std;

Heuristiques::Heuristiques(Donnees *d) {
	this->d = d;
	this->N = d->getN();
	this->M = d->getM();
	this->indMS = d->getIndMS();
	this->mans = d->getManoeuvres();
}

Heuristiques::~Heuristiques() {}

int Heuristiques::borneInfNaturelle(noeud *n, int_fast8_t nk, uint_fast8_t m) {
	int resultat = n->coutActuel + mans[m]->cout;

	int_fast8_t *indP = n->indP;
	uint_fast8_t *etat = n->etat;

	uint_fast8_t indNi;
	uint_fast8_t indNj;
	uint_fast8_t indMJ;

	bool realisable = true;
	indNj = 0;

	while ((indNj < N) && (realisable)) {
		uint_fast8_t nj = n->indP[indNj];
		if ((nj == -1) && (nj != nk)) {
			bool manTrouvee = false;
			uint_fast8_t indMj = 0;
			while ((indMj < M) && !manTrouvee) {
				uint_fast8_t mj = indMS[indMj];
				indNi = 0;
				uint_fast8_t ni = indP[indNi];
				while ((indNi < N) && ((ni == -1) || (!d->sontEnConflit(ni, nj, etat[ni], mj)))) {
					++indNi;
					ni = indP[indNi];
				}
				if (indNi == N) {
					if (!d->sontEnConflit(nk, nj, m, mj)) {
						manTrouvee = true;
						resultat += mans[mj]->cout;
					}
				}
				++indMj;
			}
			if (indMj == M) {
				realisable = false;
				resultat = -1;
			}
		}
		++indNj;
	}

	return resultat;
}

uint_fast8_t *Heuristiques::completionGloutonne(noeud *n, int *obj) {
	*obj = n->coutActuel;
	uint_fast8_t *sol = (uint_fast8_t *) malloc(N*sizeof(uint_fast8_t));
	for (int i = 0; i < N; ++i) {
		if (n->indP[i] != -1) {
			sol[i] = n->etat[n->indP[i]];
		} else {
			sol[i] = M;
		}
	}

	bool gloutonSucces = true;
	uint_fast8_t nj = 0;
	while ((nj < N) && gloutonSucces) {
		if (sol[nj] == M) {
			uint_fast8_t indM = 0;
			bool manTrouvee = false;
			while ((indM < M) && !manTrouvee) {
				uint_fast8_t m = indMS[indM];
				uint_fast8_t ni = 0;
				while ((ni < N) && ((sol[ni] == M) || !d->sontEnConflit(ni, nj, sol[ni], m))) {
					++ni;
				}
				if (ni == N) {
					manTrouvee = true;
					sol[nj] = m;
					*obj = *obj + mans[m]->cout;
				}
				++indM;
			}
			if (!manTrouvee) {
				free(sol);
				gloutonSucces = false;
				*obj = -1;
			}
		}
		++nj;
	}

	return sol;
}

// AVEC DES SWAPS COMME N QUEEN

// 1) Affecter à chaque avion la maneuvre la moins coûteuse (réalisable selon l'état courant ?)

// 2) Tant que la solution n'est pas admissible, changer l'affectation d'un avion de manière à rendre l'avion admissible par rapport aux autres et à augmenter le moins possible le delta

// ! : on ne trouve pas forcément une solution admissible

// Une fois qu'on a une solution admissible : possibiltié d'améliorer encore un peu

/*uint_fast8_t *Heuristiques::solutionGloutonne(int *obj) {
1	uint_fast8_t *etat = (uint_fast8_t *) malloc(N*sizeof(uint_fast8_t));
	*obj = 0;
}*/



// TODO : Après obtention d'une borne supérieure globale, il sera possible de supprimer des noeuds dans le tas (Un appel après chaque doublement de taille ?)

// QUESTIONNEMENT : On affecte les avions selon leur plus petit domaine, plus pertinent selon plus grand nombre de contraintes avec les avions non fixés ?