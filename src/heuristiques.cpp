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
	// TODO
	return coutActuel + mans[m]->cout;
}