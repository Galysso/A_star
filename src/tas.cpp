#include "tas.hpp"

#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace std;

int plusDeux(int size) {
	return size+2;
}

Tas::Tas(int maxSize, int UB) {
	assert(maxSize > 0);

	this->UB = UB;
	this->size = 0;
	this->maxSize = maxSize;
	this->pile = (noeud **) malloc(maxSize*sizeof(noeud *));
}

Tas::~Tas() {
	free(this->pile);
}

void Tas::empiler(noeud *n) {
	if (size == maxSize) {
		maxSize *= 2;
		pile = (noeud **) realloc(pile, maxSize*sizeof(noeud *));
	}

	pile[size] = n;

	int ind = size;
	int indPere = (ind-1)/2;
	double nBorneInf = n->selection;

	while ((pile[indPere]->selection > nBorneInf) || (pile[indPere]->selection == nBorneInf) && (pile[indPere]->prof < n->prof)) {
		noeud *temp = pile[indPere];
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
	noeud *nDesc = pile[size];
	double nVal = nDesc->selection;
	pile[0] = nDesc;
	int ind = 0;
	int indMinF = -1;

	if (2 < size) {
		if (pile[1]->selection < pile[2]->selection) {
			indMinF = 1;
		} else {
			indMinF = 2;
		}
	} else if (1 < size) {
		indMinF = 1;
	}

	while ((indMinF != -1) && ((pile[indMinF]->selection < nVal) || ((pile[indMinF]->selection == nVal) && (pile[indMinF]->prof > nDesc->prof)))) {
		pile[ind] = pile[indMinF];
		pile[indMinF] = nDesc;
		ind = indMinF;
		int indFG = (ind*2)+1;
		int indFD = (ind+1)*2;

		if (indFD < size) {
			if (pile[indFG]->selection < pile[indFD]->selection) {
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

noeud *Tas::top() {
	assert(size > 0);

	return pile[0];
}

int Tas::getN() {
	return size;
}

void Tas::setUB(int UB) {
	this->UB = UB;
}