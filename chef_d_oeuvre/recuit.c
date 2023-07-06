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


Regle genererRegleAlea()
{
	Regle regle;
	for (int i = 0; i < 5; i++)
	{
		int randRegle = (rand() % NB_ETAT)-1;
		regle.observ.routes[i] = randRegle;
	}

	int randDecis = (rand() % 3)-1;
	regle.decis = randDecis;

	int randPrio = (rand() % 10)+1;
	regle.priorite = randPrio;

	regle.obsPiece.colonne = (rand()%5) -2;
	regle.obsPiece.presence = (rand()% NB_ETAT)-1;

	return regle;
	
}

TabRegle generer_solution_initiale() {
	TabRegle tabRegle;
	for (int i = 0; i < NB_REGLES; i++)
	{
		tabRegle.regles[i] = genererRegleAlea();
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
	int random_num = rand() % 9;  // Génère un nombre aléatoire entre 0 et 6 

	if (random_num < 5) { //Modif observation
		int iObs = rand() % 5;
		int observ = tabRegle.regles[iRegle].observ.routes[iObs];
		if (observ!=JOKER && rand()%3) {
			tabRegle.regles[iRegle].observ.routes[iObs] = JOKER;
			return tabRegle;
		}
		tabRegle.regles[iRegle].observ.routes[iObs] = rand_dif(observ, -1, 4);
	} else if (random_num == 5) { //Modif deplacement
		tabRegle.regles[iRegle].decis = rand_dif(tabRegle.regles[iRegle].decis, -1, 1);
	} else if (random_num == 6){ //Modif priorité
		tabRegle.regles[iRegle].priorite = rand_dif(tabRegle.regles[iRegle].priorite, 1, 10);
	}
	else if (random_num == 7) {
		tabRegle.regles[iRegle].obsPiece.colonne = rand_dif(tabRegle.regles[iRegle].obsPiece.colonne, -2, 2);
	}
	else {
		if (tabRegle.regles[iRegle].obsPiece.presence!=JOKER && rand()%3) {
			tabRegle.regles[iRegle].obsPiece.presence = JOKER;
		}
		else {
			tabRegle.regles[iRegle].obsPiece.presence = rand_dif(tabRegle.regles[iRegle].obsPiece.presence, -1, 4);
		}
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


	for (int i=0; i<NUM_THREADS; ++i) {
		threads_handles[i] = 0;
		retours[i] = 0;
	}
	struct ParamsBoucleIA params = {
		.tabRegle = tabRegle,
		.rect_fenetre = *rect_fenetre,
		.nb_parties = nb_parties/NUM_THREADS
	};

    for (int i = 0; i < NUM_THREADS; i++) {
        if (thrd_create(&threads_handles[i], wrap_boucle_ia, (void *)&params) != thrd_success) {
			fprintf(stderr, "Erreur création thread\n");
		}
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

