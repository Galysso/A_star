#ifndef HEURISTIQUES_HPP
#define HEURISTIQUES_HPP

#include "donnees.hpp"
#include "a_star.hpp"

#include <cstdint>
#include <iostream>
#include <cstdlib>

//struct noeud;

class Heuristiques {
	private:
		Donnees *d;
		uint_fast8_t *indMS;
		manoeuvre **mans;
		int N;
		int M;

	public:
		Heuristiques(Donnees *d);
		~Heuristiques();
		int borneInfNaturelle(noeud *n, uint_fast8_t m);
		//uint_fast8_t *solutionGloutonne(int *obj);
};

#endif