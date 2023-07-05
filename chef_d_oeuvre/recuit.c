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
		int randRegle = (rand() % NB_ETAT)-1;
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

int rand_dif(int base, int mini, int maxi) {
	int mod = maxi - mini +1;
	int nouveau = (rand()%mod) + mini;
	if (nouveau==base) {
		nouveau = ((nouveau+1-mini)%mod) + mini;
	}
	return nouveau;
}

TabRegle alterTabRegle(TabRegle tabRegle) {
	int iRegle = rand() % NB_REGLES;
	int random_num = rand() % 7;  // Génère un nombre aléatoire entre 0 et 6 

	if (random_num < 5) { //Modif observation
		int iObs = rand() % 5;
		int observ = tabRegle.regles[iRegle].observ.routes[iObs];
		tabRegle.regles[iRegle].observ.routes[iObs] = rand_dif(observ, -1, 4);
	} else if (random_num == 5) { //Modif deplacement
		tabRegle.regles[iRegle].decis = rand_dif(tabRegle.regles[iRegle].decis, -1, 1);
	} else { //Modif priorité
		tabRegle.regles[iRegle].priorite = rand_dif(tabRegle.regles[iRegle].priorite, 1, 10);
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


bool recuit_impl(TabRegle* tab, const int scoreTab, double temperature, int* nouveaux_score, SDL_Rect * rect_fenetre, size_t nb_parties, unsigned seed_base) {
	TabRegle nouvTab = alterTabRegle(*tab);
	srand(seed_base);
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
	double temperature = init_temperature(rect_fenetre)/6;
	printf("température initiale : %f\n", temperature);
	// afficheChemin(&chemin);
	double raison = 0.997;//pow(-temperature, (1.0/nombre_iterations)-1);
	
	unsigned int seed_base = rand();
	srand(seed_base);
	int scoreJeu = multi_boucle_ia(tabRegle, rect_fenetre, nb_parties);
	printf("raison : %f\n", raison);
	int it = 0;
	int last_checkscore = 0;

	while (it<nombre_iterations) {
		int nouveux_score;
		if (recuit_impl(&tabRegle, scoreJeu, temperature-temperature_min, &nouveux_score, rect_fenetre, nb_parties, seed_base)) {
			scoreJeu = nouveux_score;
		}
		if (it%(1+nombre_iterations/20) == 0) {
			if (last_checkscore == scoreJeu) {
				temperature -= 600;
			}
			last_checkscore = scoreJeu;
			printf("Iteration : %d  Score : %d Température %f\n", it, scoreJeu/175, temperature-temperature_min);
		}
		it++;
		temperature *= raison;
	}

	printf("Score : %d\n", scoreJeu/175);
	return tabRegle;
}

