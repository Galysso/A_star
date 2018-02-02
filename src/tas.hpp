#ifndef TAS_HPP
#define TAS_HPP

#include "a_star.hpp"

class Tas {
	private:
		int size;
		int maxSize;
		noeud **pile;
		int UB;

	public:
		Tas(int maxSize, int UB);
		~Tas();
		void empiler(noeud *n);
		void depiler();
		noeud *top();
		int getN();
		void setUB(int UB);
};

#endif