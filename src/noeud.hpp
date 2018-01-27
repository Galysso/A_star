#ifndef NOEUD_HPP
#define NOEUD_HPP

class Noeud {
	private:
		// plein d'autres choses
		double borneInf;

	public:
		Noeud(double borneInf);
		~Noeud();
		double getBorneInf();
};

#endif