#include "heuristiques.hpp"
#include "donnees.hpp"

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

int Heuristiques::borneInfNaturelle(uint_fast8_t *etat, uint_fast8_t prof, uint_fast8_t m, int coutActuel) {
	int resultat = coutActuel + mans[m]->cout;
	int nj = prof+1;
	bool realisable = true;
	bool manTrouvee = false;

	while ((nj < N) && realisable) {
		int indmj = 0;
		while ((indmj < M) && (!manTrouvee)) {
			int ni = 0;
			while ((ni < prof) && !d->sontEnConflit(ni, nj, etat[ni], indMS[indmj])) {
				++ni;
			}
			if ((ni == prof) && (!d->sontEnConflit(prof, nj, m, indMS[indmj]))) {
				resultat += mans[indMS[indmj]]->cout;
				manTrouvee = true;
			} else {
				manTrouvee = false;
			}
			++indmj;
		}
		realisable = (indmj != M);
		++nj;
	}

	return resultat;
}

/*uint_fast8_t *Heuristiques::solutionGloutonne(int *obj) {
	uint_fast8_t *etat = (uint_fast8_t *) malloc(N*sizeof(uint_fast8_t));
	*obj = 0;
}*/