#include "recuit.h"
#include "boucle_entrainement_ia.h"
#include "regle.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <threads.h>


TabRegle generer_solution_initiale() {
	TabRegle tabRegle;
	for (int i = 0; i < NB_REGLES; i++)
	{
		tabRegle.regles[i] = genererRegleAlea();
	}
	return tabRegle;
}

bool recuit_impl(TabRegle* tab, const int scoreTab, double temperature, int* nouveaux_score, SDL_Rect * rect_fenetre, size_t nb_parties) {
	TabRegle nouvTab = alterTabRegle(*tab);
	*nouveaux_score = multi_boucle_ia(nouvTab, rect_fenetre, nb_parties);
	
	if (-scoreTab > -(*nouveaux_score)) {
		*tab = nouvTab; //chemin->val = nouveauChemin.val;
		return true;
	}
	else {
		float p = exp(-(float)(*nouveaux_score - scoreTab)/temperature);
		if ((double)rand() /RAND_MAX < p) {
			*tab = nouvTab;
			return true;	
		}
		else {
			return false;
		}
	}
}


double init_temperature(SDL_Rect * rect_fenetre) {
	TabRegle tab = generer_solution_initiale();
	int score_max = boucle_ia(false, tab, rect_fenetre, NULL, NULL);

	for (size_t i=0; i<20; ++i) {
		tab = generer_solution_initiale();
		int score = boucle_ia(false, tab, rect_fenetre, NULL, NULL);
		if (score > score_max) { //comme le recuit est une minimisation on fait l'opposé du score (on veut maximiser)
			score_max = score;
		}
	}

	return -score_max;
}


TabRegle recuit(int nombre_iterations,SDL_Rect * rect_fenetre, size_t nb_parties,TabRegle tabRegle) {

	double temperature_min = 600;
	double temperature = init_temperature(rect_fenetre)/8;
	printf("température initiale : %f\n", temperature);
	// afficheChemin(&chemin);
	double raison = 0.995;//pow(-temperature, (1.0/nombre_iterations)-1);
	int scoreJeu = multi_boucle_ia(tabRegle, rect_fenetre, nb_parties);
	printf("raison : %f\n", raison);
	int it = 0;
	int last_checkscore = 0;

	while (it<nombre_iterations) {
		int nouveux_score;
		if (recuit_impl(&tabRegle, scoreJeu, temperature-temperature_min, &nouveux_score, rect_fenetre, nb_parties)) {
			scoreJeu = nouveux_score;
		}
	
		if (it%(1+nombre_iterations/20) == 0) {
			printf("Iteration : %d  Score : %d Température %f\n", it, scoreJeu/175, temperature-temperature_min);
		}
		it++;
		if (it%60 == 0) {
			if (last_checkscore == scoreJeu) {
				temperature -= 600;
			}
			last_checkscore = scoreJeu;
		}
		temperature *= raison;
	}

	printf("Score : %d\n", scoreJeu/175);
	return tabRegle;
}

