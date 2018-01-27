#include "tas.hpp"
#include "noeud.hpp"

#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace std;

int plusDeux(int size) {
	return size+2;
}

Tas::Tas(int maxSize) {
	assert(maxSize > 0);

	this->size = 0;
	this->maxSize = maxSize;
	this->pile = (Noeud **) malloc(maxSize*sizeof(Noeud*));
}

Tas::~Tas() {
	free(this->pile);
}

void Tas::empiler(Noeud *n) {
	if (size == maxSize) {
		maxSize *= 2;
		pile = (Noeud **) realloc(pile, maxSize*sizeof(Noeud *));
	}

	pile[size] = n;

	int ind = size;
	int indPere = (ind-1)/2;
	double nBorneInf = n->getBorneInf();

	while (pile[indPere]->getBorneInf() > nBorneInf) {
		Noeud *temp = pile[indPere];
		pile[indPere] = n;
		pile[ind] = temp;
		ind = indPere;
		indPere = (ind-1)/2;
	}

	++size;
}

void Tas::depiler() {
	assert(size > 0);

	--size;
	Noeud *nDesc = pile[size];
	double nVal = nDesc->getBorneInf();
	pile[0] = nDesc;
	int ind = 0;
	int indMinF = -1;

	if (2 < size) {
		if (pile[1]->getBorneInf() < pile[2]->getBorneInf()) {
			indMinF = 1;
		} else {
			indMinF = 2;
		}
	} else if (1 < size) {
		indMinF = 1;
	}

	while ((indMinF != -1) && (pile[indMinF]->getBorneInf() < nVal)) {
		pile[ind] = pile[indMinF];
		pile[indMinF] = nDesc;
		ind = indMinF;
		int indFG = (ind*2)+1;
		int indFD = (ind+1)*2;

		if (indFD < size) {
			if (pile[indFG]->getBorneInf() < pile[indFD]->getBorneInf()) {
				indMinF = indFG;
			} else {
				indMinF = indFD;
			}
		} else if (indFG < size) {
			indMinF = indFG;
		} else {
			indMinF = -1;
		}
	}
}

Noeud *Tas::top() {
	assert(size > 0);

	return pile[0];
}

int Tas::getN() {
	return size;
}