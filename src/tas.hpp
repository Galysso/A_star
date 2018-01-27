#ifndef TAS_HPP
#define TAS_HPP

#include "noeud.hpp"

class Tas {
	private:
		int size;
		int maxSize;
		Noeud **pile;

	public:
		Tas(int maxSize);
		~Tas();
		void empiler(Noeud *n);
		void depiler();
		Noeud *top();
		int getN();

};

#endif