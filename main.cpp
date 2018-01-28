#include "src/tas.hpp"
#include "src/noeud.hpp"
#include "src/donnees.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 4) {

		cerr << "Le programme prend en paramètre 3 entiers, exemple :" << endl;
		cerr << "\"./main 40 2 5\" lance le programme avec l'instance cluster_40ac_2err_5." << endl;

	} else {
		time_t debutLect, finLect, debutRes, finRes;
		srand(time(NULL));
		cout << "Lecture des donnees..." << endl;
		debutLect = clock();
		Donnees d(argv[1], argv[2], argv[3]);
		finLect = clock();
		cout << "Temps de lecture : " << ((double)(finLect-debutLect)/CLOCKS_PER_SEC) << "s" << endl;

		int i = 1;
		int j = 7;
		int mi = 35;
		int mj = 56;
		cout << "conflits(" << i << "," << j << "," << mi << "," << mj << ")=" << d.sontEnConflit(i,j,mi,mj) << endl;

		i = 5;
		j = 11;
		mi = 55;
		mj = 96;
		cout << "conflits(" << i << "," << j << "," << mi << "," << mj << ")=" << d.sontEnConflit(i,j,mi,mj) << endl;

		getchar();
		cout << "Fin" << endl;
	}
}