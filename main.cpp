#include "src/tas.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
	srand(time(NULL));

	Tas tas(2);
	int x;
	for (int i = 0; i < 25; ++i) {
		x = rand()%100;
		tas.empiler(x);
		cout << "insertion : " << x << " (" << tas.top() << ")" << endl;
	}

	while (tas.getN() > 0) {
		cout << "tas.top : " << tas.top() << endl;
		tas.depiler();
	}

	cout << "COCO" << endl;
	return 0;
}