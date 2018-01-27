#include "noeud.hpp"

#include <cassert>
#include <iostream>
#include <cstdlib>

Noeud::Noeud(double borneInf) {
	this->borneInf = borneInf;
}

Noeud::~Noeud() {}

double Noeud::getBorneInf() {
	return borneInf;
}