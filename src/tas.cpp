#include "tas.hpp"
#include <cassert>
#include <iostream>
#include <cstdlib>

using namespace std;

int plusDeux(int n) {
	return n+2;
}

Tas::Tas(int maxN) {
	assert(maxN > 0);

	this->N = 0;
	this->maxN = maxN;
	this->pile = (int *) malloc(maxN*sizeof(int));
}

Tas::~Tas() {
	free(this->pile);
}

void Tas::empiler(int x) {
	if (N == maxN) {
		maxN *= 2;
		pile = (int *) realloc(pile, maxN*sizeof(int));
	}

	pile[N] = x;

	int n = N;
	int nPere = (n-1)/2;

	while (pile[nPere] > pile[n]) {
		int temp = pile[nPere];
		pile[nPere] = x;
		pile[n] = temp;

		n = nPere;
		nPere = (n-1)/2;
	}

	++N;
}

void Tas::depiler() {
	assert(N > 0);

	--N;
	int vN = pile[N];
	pile[0] = vN;

	int n = 0;
	int indMinF = -1;
	if (2 < N) {
		if (pile[1] < pile[2]) {
			indMinF = 1;
		} else {
			indMinF = 2;
		}
	} else if (1 < N) {
		indMinF = 1;
	}

	while ((indMinF != -1) && (pile[indMinF] < vN)) {
		pile[n] = pile[indMinF];
		pile[indMinF] = vN;

		n = indMinF;
		int indFG = (n*2)+1;
		int indFD = (n+1)*2;

		if (indFD < N) {
			if (pile[indFG] < pile[indFD]) {
				indMinF = indFG;
			} else {
				indMinF = indFD;
			}
		} else if (indFG < N) {
			indMinF = indFG;
		} else {
			indMinF = -1;
		}
	}
}

int Tas::top() {
	assert(N > 0);

	return pile[0];
}

int Tas::getN() {
	return N;
}