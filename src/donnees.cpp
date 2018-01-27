#include "donnees.hpp"

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Donnees::Donnees(char *a, char *b, char *c) {
	string filename = string("donnees/conflits/cluster_") + string(a) + string("ac_") + string(b) + string("err_") + string(c);

	ifstream file(filename.c_str());

	if (!file.is_open()) {
		cout << "Le fichier \"" << filename << "\" n'a pas pu être ouvert." << endl;
	} else {
		string s;
		// LECTURE DE LA TAILLE DE L'INSTANCE
		file >> s;
		file >> N;
		file >> M;
		file >> s; file >> s; file >> s;

		// ALLOCATION MEMOIRE DES MANEUVRES
		mans = (maneuvre **) malloc(M*sizeof(maneuvre *));
		// ALLOCATION MEMOIRE DES CONFLITS
		confs = (bool ****) malloc((N-1)*sizeof(bool ***));
		checkNG = (bool ***) malloc((N-1)*sizeof(bool ***));
		for (int i = 0; i < N-1; ++i) {
			confs[i] = (bool ***) malloc(N*sizeof(bool **));
			checkNG[i] = (bool **) malloc(N*sizeof(bool *));
			for (int j = i+1; j < N; ++j) {
				confs[i][j] = (bool **) malloc(M*sizeof(bool *));
				checkNG[i][j] = (bool *) malloc(M*sizeof(bool));
				for (int mi = 0; mi < M; ++mi) {
					checkNG[i][j][mi] = false;
				}
			}
		}
		do {
			if (s == string("c")) {
				// LECTURE DES CONFLITS
				int i, j, mi, mj;
				file >> i;
				file >> j;
				file >> mi;
				file >> s;
				confs[i][j][mi] = (bool *) malloc(M*sizeof(bool));
				checkNG[i][j][mi] = true;
				do {
					mj = stoi(s);
					confs[i][j][mi][mj] = true;
					file >> s;
				} while ((s != string("c")) && (s != string("m")));
			} else if (s == string("m")) {
				// LECTURE DES MANEUVRES
				int m;
				file >> m;
				maneuvre *man = (maneuvre *) malloc(sizeof(maneuvre));
				file >> man->cout;
				file >> man->d0;
				file >> man->d1;
				file >> man->alpha;
				mans[m] = man;
				file >> s;
			} else {
				file >> s;
			}
		} while (!file.eof());
	}
}

Donnees::~Donnees() {}

int Donnees::getN() {
	return N;
}

int Donnees::getM() {
	return M;
}

maneuvre *Donnees::getManeuvre(int m) {
	return mans[m];
}

bool Donnees::sontEnConflit(int i, int j, int mi, int mj) {
	assert(i < j);

	if (checkNG[i][j][mi]) {
		return confs[i][j][mi][mj];
	} else {
		return 0;
	}
}