#include "recuit.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


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

/*
Chemin alterChemin(const Chemin* chemin) {
	int i = rand()%(chemin->nombre_elems-1);
	int	j = 1 + rand()%(chemin->nombre_elems-2);
	if (j == i) {
		j = 1+(j+1)%(chemin->nombre_elems-2); 
	}


	Chemin nouveau_chemin = {
		.val = malloc(sizeof(int)*chemin->nombre_elems),
		.nombre_elems = chemin->nombre_elems,
	};
	
	memcpy(nouveau_chemin.val, chemin->val, sizeof(int)*chemin->nombre_elems);
	
	nouveau_chemin.val[i] = chemin->val[j];
	nouveau_chemin.val[j] = chemin->val[i];
	if (i==0) {
		nouveau_chemin.val[chemin->nombre_elems-1] = chemin->val[j];
	}
	return nouveau_chemin;
}


bool recuit_impl(Chemin* chemin, const int longueurChemin, const Matrice matrice, double temperature, int* nouvelle_longeur) {
	Chemin nouveauChemin = alterChemin(chemin);
	*nouvelle_longeur = calculDistanceGrapheComplet(matrice, &nouveauChemin);
	
	if (longueurChemin > *nouvelle_longeur) {
		free(chemin->val);
		chemin->val = nouveauChemin.val;
		return true;
	}
	else {
		float p = exp(-(float)(*nouvelle_longeur - longueurChemin)/temperature);
		if ((double)rand() /INT32_MAX < p) {
			free(chemin->val);
			chemin->val = nouveauChemin.val;
			return true;	
		}
		else {
			return false;
		}
	}
}



double init_temperature(Matrice matrice, size_t nombre_noeud) {
	Chemin chemin = generer_solution_initiale(nombre_noeud);
	int longeur_max = calculDistanceGrapheComplet(matrice, &chemin);
	for (size_t i=0; i<20; ++i) {
		fisherYatesMelange(&chemin);
		int longeur = calculDistanceGrapheComplet(matrice, &chemin);
		if (longeur > longeur_max) {
			longeur_max = longeur;
		}
	}

	return longeur_max;
}

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

int recuit(Matrice matrice, int N, int nombre_iterations) {
	if (N <= 1) {
		return 0;
	}

	double temperature = init_temperature(matrice, N);
	printf("température initiale : %f\n", temperature);
	double pente = temperature/nombre_iterations;
	Chemin chemin = generer_solution_initiale(N);
	afficheChemin(&chemin);
	int longeurChemin = calculDistanceGrapheComplet(matrice, &chemin);

	while (temperature>0.001) {
		int nouvelle_longeur;
		if (recuit_impl(&chemin, longeurChemin, matrice, temperature, &nouvelle_longeur)) {
			longeurChemin = nouvelle_longeur;
		}
			
		temperature -= pente;
	}

	afficheChemin(&chemin);
	printf("Longueur %d\n", longeurChemin);

	free(chemin.val);
	return longeurChemin;
}
*/
