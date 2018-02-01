#ifndef A_STAR_HPP
#define A_STAR_HPP

#include "donnees.hpp"

typedef struct {
	int borneInf;
	int coutActuel;
	uint_fast8_t prof;
	uint_fast8_t *etat;
	int_fast8_t *indP;
	int selection;
} noeud;

class A_star {
	private:
		bool resolu;
		Donnees *d;
		uint_fast8_t *bestSol;
		int bestObj;
		uint_fast8_t N;
		uint_fast8_t M;

	public:
		A_star(Donnees *d);
		~A_star();
		int plusPetitDomaine(noeud *n);
		void resoudre();
		uint_fast8_t *getSol();
		int getObj();
};

#endif