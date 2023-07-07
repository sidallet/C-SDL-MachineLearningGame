#include "genetique.h"
#include "boucle_entrainement_ia.h"
#include "regle.h"
#include <SDL2/SDL_rect.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void generer_population_initiale(TabRegle population[]) {
	for (int s=0; s<TAILLE_POPULATION; ++s) {
		for (size_t i=0; i<NB_REGLES; ++i) {
			population[s].regles[i] = genererRegleAlea();
		}
	}
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

Regle fusion_regle(Regle r1, Regle r2) {
	int randi = rand()%(5+2+1+1);
	Regle rfus;
	rfus.decis = r2.decis;
	rfus.obsPiece = r2.obsPiece;
	rfus.priorite = r2.priorite;

	if (randi<5) {
		for (size_t i=0; i<randi; ++i) {
			rfus.observ.routes[i] = r1.observ.routes[i];
		}
		for (size_t i=randi; i<5; ++i) {
			rfus.observ.routes[i] = r2.observ.routes[i];
		}
	}
	else {
		for (size_t i=0; i<5; ++i) {
			rfus.observ.routes[i] = r1.observ.routes[i];
		}

		switch (randi) {
			case 5: {
				rfus.obsPiece.colonne = r1.obsPiece.colonne;
				break;
			}
			case 6: {
				rfus.obsPiece = r1.obsPiece;
				break;
			}
			case 7: {
				rfus.obsPiece = r1.obsPiece;
				rfus.decis = r1.decis;
				break;
			}
			case 8: {
				rfus.obsPiece = r1.obsPiece;
				rfus.decis = r1.decis;
				rfus.priorite = r1.priorite;
				break;
			}
		
		}		
	}
	return rfus;
}

TabRegle fusion_parent(const TabRegle parent1, const TabRegle parent2) {
	TabRegle fils;
	for (size_t i=0; i<NB_REGLES; ++i) {
		if (rand()%2==0) {
			fils.regles[i] = fusion_regle(parent1.regles[i], parent2.regles[i]);		
		}
		else {
			fils.regles[i] = fusion_regle(parent2.regles[i], parent1.regles[i]);		
		}
	}

	return fils;
}

void generer_enfants(TabRegle population[]) {
	for (size_t i=NOMBRE_PARENTS; i<TAILLE_POPULATION; ++i) {
		population[i] = fusion_parent(population[i%NOMBRE_PARENTS], population[(i+rand())%NOMBRE_PARENTS]);
	}
}

void muter_population(TabRegle population[], int scores[]) {
	for (size_t i=0; i<TAILLE_POPULATION; ++i) {
		if (rand()%20 == 0){
			population[i] = copier_tab_regle(alterTabRegle(population[i]));
			scores[i] = 0;
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

TabRegle genetique(int nombre_iterations, SDL_Rect * rect_fenetre, size_t nb_parties, TabRegle population[TAILLE_POPULATION]) {
	int scores[TAILLE_POPULATION];
	init_score(scores);
	calculerScorePopulation(population, scores, rect_fenetre, nb_parties);

	for (int s=0; s<TAILLE_POPULATION; ++s) {
		printf("%d ", scores[s]/175);	
	}
	printf("\n");	

	printf("%d\n", scores[selection_meilleur(scores)]/175);
	for(size_t i=0; i<nombre_iterations; ++i) {
		selectionner_parents(population, scores);

		generer_enfants(population);
	
		muter_population(population, scores);
		calculerScorePopulation(population, scores, rect_fenetre, nb_parties);


		for (int s=0; s<NOMBRE_PARENTS; ++s) {
			printf("%d ", scores[s]/175);	
		}
		printf("\n");	

		printf("%d\n", scores[selection_meilleur(scores)]/175);
	}

	for (int s=0; s<TAILLE_POPULATION; ++s) {
		printf("%d ", scores[s]/175);	
	}
	printf("\n");	



	return population[selection_meilleur(scores)];
}


