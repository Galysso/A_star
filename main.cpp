#include "src/tas.hpp"
#include "src/donnees.hpp"
#include "src/a_star.hpp"
#include "src/heuristiques.hpp"

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
		srand(time(NULL));
		// VARIABLE DE CHRONOMETRE
		time_t debutLect, finLect, debutRes, finRes;

		// LECTURE DES DONNEES
		debutLect = clock();
		Donnees *d = new Donnees(argv[1], argv[2], argv[3]);
		finLect = clock();
		//cout << "Temps de lecture : " << ((double)(finLect-debutLect)/CLOCKS_PER_SEC) << "s" << endl;

		// LANCEMENT DE L'ALGORITHME
		A_star *algo = new A_star(d);
		algo->resoudre();
	}
}