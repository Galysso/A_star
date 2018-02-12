#include "heuristiques.hpp"
#include "donnees.hpp"
#include "a_star.hpp"

#include <glpk.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>

using namespace std;

Heuristiques::Heuristiques(Donnees *d) {
	this->d = d;
	this->N = d->getN();
	this->M = d->getM();
	this->indMS = d->getIndMS();
	this->mans = d->getManoeuvres();
	this->nbNG = d->getNbNG();

	this->ia = (int *) malloc((1 + N*M + 2*nbNG)*sizeof(int));
	this->ja = (int *) malloc((1 + N*M + 2*nbNG)*sizeof(int));
	this->ar = (double *) malloc((1 + N*M + 2*nbNG)*sizeof(double));

	this->glpProb = glp_create_prob();
	glp_set_obj_dir(glpProb, GLP_MIN);
	glp_add_cols(glpProb, N*M);
	glp_add_rows(glpProb, N + nbNG + N);

	for (int i = 0; i < N; ++i) {
		glp_set_row_bnds(glpProb, i+1, GLP_LO, 1.0, 0.0);
	}
	for (int i = 0; i < nbNG; ++i) {
		glp_set_row_bnds(glpProb, N+i+1, GLP_UP, 0.0, 1.0);
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			glp_set_col_kind(glpProb, i*M + j + 1, GLP_CV);
			glp_set_col_bnds(glpProb, i*M + j + 1, GLP_DB, 0.0, 1.0);
			glp_set_obj_coef(glpProb, i*M + j + 1, mans[j]->cout);
		}
	}


	int cpt = 1;

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j) {
			ia[cpt] = i+1;
			ja[cpt] = i*M+j+1;
			ar[cpt] = 1;
			++cpt;
		}
	}

	int line = N+1;

	for (int ni = 0; ni < N; ++ni) {
		for (int nj = ni+1; nj < N; ++nj) {
			for (int mi = 0; mi < M; ++mi) {
				for (int mj = 0; mj < M; ++mj) {
					if (d->sontEnConflit(ni, nj, mi, mj)) {
						ia[cpt] = line;
						ja[cpt] = ni*M + mi + 1;
						ar[cpt] = 1;
						++cpt;
						ia[cpt] = line;
						ja[cpt] = nj*M + mj + 1;
						ar[cpt] = 1;
						++cpt;
						++line;
					}
				}
			}
		}
	}

	glp_term_out(false);
	glp_load_matrix(glpProb, N*M + 2*nbNG, ia, ja, ar);
	//glp_write_lp(glpProb, NULL, "modelisation");
	//glp_simplex(glpProb, NULL);
	
	//cout << "cout optimal = " <<  glp_mip_obj_val(glpProb) << endl;

}

Heuristiques::~Heuristiques() {}

int Heuristiques::borneInfNaturelle(noeud *n, int_fast8_t nk, uint_fast8_t m) {
	int resultat = n->coutActuel + mans[m]->cout;

	int_fast8_t *indP = n->indP;
	uint_fast8_t *etat = n->etat;

	uint_fast8_t indNi;
	uint_fast8_t indNj;
	uint_fast8_t indMJ;

	bool realisable = true;
	indNj = 0;

	while ((indNj < N) && (realisable)) {
		uint_fast8_t nj = n->indP[indNj];
		if ((nj == -1) && (nj != nk)) {
			bool manTrouvee = false;
			uint_fast8_t indMj = 0;
			while ((indMj < M) && !manTrouvee) {
				uint_fast8_t mj = indMS[indMj];
				indNi = 0;
				uint_fast8_t ni = indP[indNi];
				while ((indNi < N) && ((ni == -1) || (!d->sontEnConflit(ni, nj, etat[ni], mj)))) {
					++indNi;
					ni = indP[indNi];
				}
				if (indNi == N) {
					if (!d->sontEnConflit(nk, nj, m, mj)) {
						manTrouvee = true;
						resultat += mans[mj]->cout;
					}
				}
				++indMj;
			}
			if (indMj == M) {
				realisable = false;
				resultat = -1;
			}
		}
		++indNj;
	}

	return resultat;
}

uint_fast8_t *Heuristiques::completionGloutonne(noeud *n, int *obj) {
	*obj = n->coutActuel;
	uint_fast8_t *sol = (uint_fast8_t *) malloc(N*sizeof(uint_fast8_t));
	for (int i = 0; i < N; ++i) {
		if (n->indP[i] != -1) {
			sol[i] = n->etat[n->indP[i]];
		} else {
			sol[i] = M;
		}
	}

	bool gloutonSucces = true;
	uint_fast8_t nj = 0;
	while ((nj < N) && gloutonSucces) {
		if (sol[nj] == M) {
			uint_fast8_t indM = 0;
			bool manTrouvee = false;
			while ((indM < M) && !manTrouvee) {
				uint_fast8_t m = indMS[indM];
				uint_fast8_t ni = 0;
				while ((ni < N) && ((sol[ni] == M) || !d->sontEnConflit(ni, nj, sol[ni], m))) {
					++ni;
				}
				if (ni == N) {
					manTrouvee = true;
					sol[nj] = m;
					*obj = *obj + mans[m]->cout;
				}
				++indM;
			}
			if (!manTrouvee) {
				free(sol);
				gloutonSucces = false;
				*obj = -1;
			}
		}
		++nj;
	}

	return sol;
}

int Heuristiques::borneInfGLPK(noeud *n, int_fast8_t nk, uint_fast8_t m) {
	int cpt = 1;
	uint_fast8_t prof = n->prof;
	uint_fast8_t *etat = n->etat;
	int_fast8_t *indP = n->indP;
	for (int i = 0; i < N; ++i) {
		if (indP[i] != -1) {
			glp_set_col_bnds(glpProb, i*M + etat[indP[i]] + 1, GLP_LO, 1.0, 1.0);
		}
	}
	glp_set_col_bnds(glpProb, nk*M + m + 1, GLP_LO, 1.0, 1.0);
	glp_simplex(glpProb, NULL);

	int resultat;
	//glp_term_out(true);
	//glp_write_lp(glpProb, NULL, "modelisation");
	if ((glp_get_status(glpProb) == GLP_OPT) || (glp_get_status(glpProb) == GLP_FEAS)) {
		resultat = glp_mip_obj_val(glpProb);
	} else {
		resultat = -1;
	}

	//getchar();

	for (int i = 0; i < prof; ++i) {
		glp_set_col_bnds(glpProb, i*M + etat[indP[i]] + 1, GLP_DB, 0.0, 1.0);
	}
	glp_set_col_bnds(glpProb, nk*M + m + 1, GLP_DB, 0.0, 1.0);

	return resultat;
}

// AVEC DES SWAPS COMME N QUEEN

// 1) Affecter à chaque avion la maneuvre la moins coûteuse (réalisable selon l'état courant ?)

// 2) Tant que la solution n'est pas admissible, changer l'affectation d'un avion de manière à rendre l'avion admissible par rapport aux autres et à augmenter le moins possible le delta

// ! : on ne trouve pas forcément une solution admissible

// Une fois qu'on a une solution admissible : possibiltié d'améliorer encore un peu

/*uint_fast8_t *Heuristiques::solutionGloutonne(int *obj) {
1	uint_fast8_t *etat = (uint_fast8_t *) malloc(N*sizeof(uint_fast8_t));
	*obj = 0;
}*/



// TODO : Après obtention d'une borne supérieure globale, il sera possible de supprimer des noeuds dans le tas (Un appel après chaque doublement de taille ?)

// QUESTIONNEMENT : On affecte les avions selon leur plus petit domaine, plus pertinent selon plus grand nombre de contraintes avec les avions non fixés ?