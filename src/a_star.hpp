#ifndef A_STAR_HPP
#define A_STAR_HPP

#include "donnees.hpp"
#include "heuristiques.hpp"

typedef struct {
	int borneInf;
	int coutActuel;
	uint_fast8_t prof;
	uint_fast8_t *etat;
	uint_fast8_t *indP;
} noeud;

class A_star {
	private:
		bool resolu;
		Donnees *d;
		uint_fast8_t *bestSol;
		int bestObj;

	public:
		A_star(Donnees *d);
		~A_star();
		void resoudre();
		uint_fast8_t *getSol();
		int getObj();
};

#endif