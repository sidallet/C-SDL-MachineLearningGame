#include "genetique.h"
#include "boucle_entrainement_ia.h"
#include "regle.h"
#include <SDL2/SDL_rect.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define TAILLE_POPULATION 40
#define NOMBRE_PARENTS 10

void generer_population_initiale(TabRegle population[]) {
	for (int s=0; s<TAILLE_POPULATION; ++s) {
		for (size_t i=0; i<NB_REGLES; ++i) {
			population[s].regles[i] = genererRegleAlea();
		}
	}
}

TabRegle copier_tab_regle(TabRegle tab) {
	TabRegle nouv;
	
	for (size_t i=0; i<NB_REGLES; ++i) {
		nouv.regles[i] = tab.regles[i];	
	}
	return nouv;
}

void echanger2(TabRegle arr1[], int arr2[], size_t a, size_t b) {
	TabRegle tmp1 = copier_tab_regle(arr1[a]);
	arr1[a] = copier_tab_regle(arr1[b]);
	arr1[a] = copier_tab_regle(tmp1);
	int tmp2 = arr2[a];
	arr2[a] = arr2[b];
	arr2[b] = tmp2;
}

size_t partition(TabRegle arr1[], int arrVal[], size_t gauche, size_t droite, size_t pivot) {
	int valPivot = arrVal[pivot];
	echanger2(arr1, arrVal, pivot, droite);
	size_t indiceStockage = gauche;
	for (size_t i=gauche; i<droite; ++i) {
		if (arrVal[i] > valPivot) {
			echanger2(arr1, arrVal, indiceStockage, i);
			indiceStockage++;
		}
	}
	echanger2(arr1, arrVal, droite, indiceStockage);

	return indiceStockage;
}

int select_nth_larger(TabRegle arr1[], int arrVal[], size_t n) {	
	size_t gauche = 0;
	size_t droite = TAILLE_POPULATION-1;
	size_t pivot;
	while (true) {
		if (gauche == droite) {
			return gauche;
		}
		pivot = gauche;
		pivot = partition(arr1, arrVal, gauche, droite, pivot);

		if (pivot == n) {
			return n;	
		}
		else if (n < pivot) {
			droite = pivot -1;
		}
		else {
			gauche = pivot +1;
		}
	}	
}

void selectionner_parents(TabRegle population[], int scores[]) {
	select_nth_larger(population, scores, NOMBRE_PARENTS);
	
	for (size_t i=NOMBRE_PARENTS; i<TAILLE_POPULATION; ++i) {
		scores[i] = 0;	
	}
}

TabRegle fusion_parent(const TabRegle parent1, const TabRegle parent2) {
	TabRegle fils;
	for (size_t i=0; i<NB_REGLES; ++i) {
		if (rand()%2==0) {
			fils.regles[i] = parent1.regles[i];		
		}
		else {
			fils.regles[i] = parent2.regles[i];		
		}
	}

	return fils;
}

void generer_enfants(TabRegle population[]) {
	for (size_t i=NOMBRE_PARENTS; i<TAILLE_POPULATION; ++i) {
		population[i] = fusion_parent(population[i%NOMBRE_PARENTS], population[(i+NOMBRE_PARENTS/2)%NOMBRE_PARENTS]);
	}
}

void muter_population(TabRegle population[]) {
	for (size_t i=0; i<TAILLE_POPULATION; ++i) {
		if (rand()%5 == 0){
			population[i] = copier_tab_regle(alterTabRegle(population[i]));
		}	
	}
}

size_t selection_meilleur(int scores[]) {
	size_t indice_max = 0;
	int maxi = scores[indice_max];
	for (int s=1; s<TAILLE_POPULATION; ++s) {
		if (maxi<scores[s]) {
			maxi = scores[s];
			indice_max = s;
		}
	}
	return indice_max;
}

void init_score(int scores[]) {
	memset(scores, 0, TAILLE_POPULATION * sizeof(int));
}

void calculerScorePopulation(TabRegle population[], int scores[], SDL_Rect* rect_fenetre, size_t nb_parties) {
	for (int s=0; s<TAILLE_POPULATION; ++s) {
		if (scores[s] == 0) {
			scores[s] = multi_boucle_ia(population[s], rect_fenetre, nb_parties);
		}
	}
}

TabRegle genetique(int nombre_iterations, SDL_Rect * rect_fenetre, size_t nb_parties) {
	TabRegle population[TAILLE_POPULATION];
	int scores[TAILLE_POPULATION];
	init_score(scores);
	generer_population_initiale(population);

	for(size_t i=0; i<nombre_iterations; ++i) {
		calculerScorePopulation(population, scores, rect_fenetre, nb_parties);
		selectionner_parents(population, scores);

		generer_enfants(population);
	
		muter_population(population);
		printf("%d\n", scores[selection_meilleur(scores)]/175);
	}
	
	return population[selection_meilleur(scores)];
}


