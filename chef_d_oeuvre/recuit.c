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


Regle genererRegleAlea(Regle regle)
{
	for (int i = 0; i < 5; i++)
	{
		int randRegle = (rand() % 4)-1;
		regle.observ.routes[i] = randRegle;
	}

	int randDecis = (rand() % 3)-1;
	regle.decis = randDecis;

	int randPrio = (rand() % 10)+1;
	regle.priorite = randPrio;
	return regle;
	
}

TabRegle generer_solution_initiale() {
	TabRegle tabRegle;
	Regle regle;
	for (int i = 0; i < NB_REGLES; i++)
	{
		regle = genererRegleAlea(regle);
		tabRegle.regles[i] = regle;
	}
	return tabRegle;
}


TabRegle alterTabRegle(TabRegle tabRegle) {
	int iRegle = rand() % 12;
	int random_num = rand() % 7;  // Génère un nombre aléatoire entre 0 et 14

	if (random_num < 5) { //Modif observation
		int iObs = rand() % 5;
		int randObs = (rand() % 4)-1;
		tabRegle.regles[iRegle].observ.routes[iObs] = randObs;
	} else if (random_num == 5) { //Modif deplacement
		int iDecis = (rand() % 3)-1;
		tabRegle.regles[iRegle].decis = iDecis;
	} else { //Modif priorité
		int iPrio = (rand() % 10)+1;
		tabRegle.regles[iRegle].priorite = iPrio;
	}
	return tabRegle;
}

struct ParamsBoucleIA {
	TabRegle tabRegle;
	SDL_Rect rect_fenetre;
	size_t nb_parties;
};

int wrap_boucle_ia(void* params) {
	struct ParamsBoucleIA* p = (struct ParamsBoucleIA*) params;
	int sum = 0;
	for (size_t i=0; i<p->nb_parties; ++i) {
		sum += boucle_ia(false, p->tabRegle, &p->rect_fenetre, NULL, NULL);
	}
	return sum;
}

int multi_boucle_ia(TabRegle tabRegle, SDL_Rect* rect_fenetre, size_t nb_parties) {
	int sum = 0;
	thrd_t threads_handles[NUM_THREADS];
	int retours[NUM_THREADS];

	struct ParamsBoucleIA params = {
		.tabRegle = tabRegle,
		.rect_fenetre = *rect_fenetre,
		.nb_parties = nb_parties/NUM_THREADS
	};

    for (int i = 0; i < NUM_THREADS; i++) {
        thrd_create(&threads_handles[i], wrap_boucle_ia, (void *)&params);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thrd_join(threads_handles[i], &retours[i]);
		sum += retours[i];
    }

	return sum / nb_parties;
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
		if ((double)rand() /INT32_MAX < p) {
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
		if (-score > -score_max) { //comme le recuit est une minimisation on fait l'opposé du score (on veut maximiser)
			score_max = score;
		}
	}

	return score_max;
}

/*
void fisherYatesMelange(Chemin* chemin)
{
	for (int i = 0; i < chemin->nombre_elems-1; i++)
	{
		int j = rand() % (i +1);
		int temp = chemin->val[i];
		chemin->val[i] = chemin->val[j];
		chemin->val[j] = temp;
	}
	chemin->val[chemin->nombre_elems-1] = chemin->val[0];

}
*/

TabRegle recuit(int nombre_iterations,SDL_Rect * rect_fenetre, size_t nb_parties) {
	// if (N <= 1) {
	// 	return 0;
	// }
	TabRegle tabRegle;

	double temperature = init_temperature(rect_fenetre);
	printf("température initiale : %f\n", temperature);
	double pente = temperature/nombre_iterations;
	tabRegle = generer_solution_initiale();
	// afficheChemin(&chemin);
	int scoreJeu = multi_boucle_ia(tabRegle, rect_fenetre, nb_parties);

	int it = 0;

	while (temperature>0.001) {
		int nouveux_score;
		if (recuit_impl(&tabRegle, scoreJeu, temperature, &nouveux_score, rect_fenetre, nb_parties)) {
			scoreJeu = nouveux_score;
		}
		it++;
		if (it%(1+nombre_iterations/20) == 0) {
			printf("Iteration : %d  Score : %d\n", it, scoreJeu/175);
		}
		temperature -= pente;
	}

	printf("Score : %d\n", scoreJeu/175);
	return tabRegle;
}
