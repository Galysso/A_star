#ifndef TAS_HPP
#define TAS_HPP

int plusDeux(int n);

class Tas {
	private:
		int N;
		int maxN;
		int *pile;

	public:
		Tas(int maxN);
		~Tas();
		void empiler(int x);
		void depiler();
		int top();
		int getN();

};

#endif