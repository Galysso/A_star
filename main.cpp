#include "src/tas.hpp"
#include "src/noeud.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
	srand(time(NULL));

	Tas tas(2);
	int x;
	for (int i = 0; i < 25; ++i) {
		x = rand()%1000;
		Noeud *n = new Noeud((double) x/10);
		tas.empiler(n);
		cout << "insertion : " << (double) x/10 << " (" << tas.top()->getBorneInf() << ")" << endl;
	}

	while (tas.getN() > 0) {
		cout << "tas.top : " << tas.top()->getBorneInf() << endl;
		tas.depiler();
	}

	cout << "COCO" << endl;
	return 0;
}