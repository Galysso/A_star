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
	uint nj = 0;
	bool realisable = true;
	bool manTrouvee = false;

	while ((nj < N) && realisable) {
		int indmj = 0;
		if ((n->indP[nj] == -1) && (nj != nk)) {
			while ((indmj < M) && (!manTrouvee)) {
				int ni = 0;
				while ((ni < N) && ((n->indP[ni] == -1) || !d->sontEnConflit(ni, nj, n->etat[n->indP[ni]], indMS[indmj]))) {
					++ni;
				}
				if ((ni == N) && !d->sontEnConflit(nk, nj, m, indMS[indmj])) {
					resultat += mans[indMS[indmj]]->cout;
					manTrouvee = true;
				} else {
					manTrouvee = false;
				}
				++indmj;
			}
		}
		realisable = (indmj != M);
		++nj;
	}

	return resultat;
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