#ifndef TAS_HPP
#define TAS_HPP

#include "a_star.hpp"

class Tas {
	private:
		int size;
		int maxSize;
		noeud **pile;

	public:
		Tas(int maxSize);
		~Tas();
		void empiler(noeud *n);
		void depiler();
		noeud *top();
		int getN();
};

#endif