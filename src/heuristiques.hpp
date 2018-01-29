#ifndef HEURISTIQUES_HPP
#define HEURISTIQUES_HPP

#include "donnees.hpp"

#include <cstdint>
#include <iostream>
#include <cstdlib>

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
		int borneInfNaturelle(uint_fast8_t *etat, uint_fast8_t prof, uint_fast8_t m, int coutActuel);
};

#endif