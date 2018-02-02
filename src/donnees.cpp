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
		cout << "Lecture des données..." << endl;
		string s;
		// LECTURE DE LA TAILLE DE L'INSTANCE
		file >> s;
		file >> N;
		file >> M;
		file >> s; file >> s; file >> s;

		// ALLOCATION MEMOIRE DES MANEUVRES
		mans = (manoeuvre **) malloc(M*sizeof(manoeuvre *));
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
				for (int k = 0; k < M; ++k) {
					confs[i][j][mi][k] = false;
				}
				do {
					mj = stoi(s);
					confs[i][j][mi][mj] = true;
					file >> s;
				} while ((s != string("c")) && (s != string("m")));
			} else if (s == string("m")) {
				// LECTURE DES MANOEUVRES
				int m;
				file >> m;
				manoeuvre *man = (manoeuvre *) malloc(sizeof(manoeuvre));
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


		cout << "Pré-traitement des données..." << endl;
		indMS = (uint_fast8_t *) malloc(M*sizeof(uint_fast8_t));
		for (int i = 0; i < M; ++i) {
			indMS[i] = i;
		}

		bool estTrie;
		do {
			estTrie = true;
			for (int i = 1; i < M; ++i) {
				if (mans[indMS[i-1]]->cout > mans[indMS[i]]->cout) {
					estTrie = false;
					int temp = indMS[i-1];
					indMS[i-1] = indMS[i];
					indMS[i] = temp;
				}
			}
		} while (!estTrie);
	}
}

Donnees::~Donnees() {}

int Donnees::getN() {
	return N;
}

int Donnees::getM() {
	return M;
}

manoeuvre **Donnees::getManoeuvres() {
	return mans;
}

uint_fast8_t *Donnees::getIndMS() {
	return indMS;
}

bool Donnees::sontEnConflit(int i, int j, int mi, int mj) {
	if (j < i) {
		int temp = j;
		j = i;
		i = temp;

		temp = mj;
		mj = mi;
		mi = temp;
	}

	//cout << "i=" << i << ", j=" << j << ", mi=" << mi << "mj=" << mj << endl;
	if (checkNG[i][j][mi]) {
		return confs[i][j][mi][mj];
	} else {
		return 0;
	}
}