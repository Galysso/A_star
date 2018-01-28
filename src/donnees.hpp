#ifndef DONNEES_HPP
#define DONNEES_HPP

#include <cstdint>

using namespace std;

typedef struct {
	int cout;			// Le coût de la manoeuvre
	int d0;				// L'indexe d0
	int d1;				// L'indexe d1
	int alpha;			// L'indexe alpha
} manoeuvre;

class Donnees {
	private:
		int N;						// Nombre d'avions
		int M;						// Nombre de maneuvres par avion
		manoeuvre **mans;			// La liste des maneuvres (mans[i][m] : la manoeuvre m de l'avion i)
		bool ***checkNG;			// La liste des avions et maneuvres mis en relation par la matrice creuse des conflits (checkNG[i][j][mj] = 1 si il existe des maneuvres mj de l'avion j incompatibles avec la manoeuvre mi de l'avion i)
		bool ****confs;				// La liste des maneuvres incompatibles (conflits[i][j][mi][mj] : 1 si la manoeuvre mj de l'avion j est incompatible avec la maneuve mi de l'avion i)

	public:
		Donnees(char *a, char *b, char *c);
		~Donnees();
		int getN();
		int getM();
		manoeuvre *getManeuvre(int m);
		bool sontEnConflit(int i, int j, int mi, int mj);
};

#endif