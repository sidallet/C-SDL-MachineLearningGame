#include "genetique.h"
#include "regle.h"

#define TAILLE_POPULATION 40
#define NOMBRE_PARENTS 20



TabRegle genetique(int nombre_iterations, SDL_Rect * rect_fenetre, size_t nb_parties) {
	TabRegle population[TAILLE_POPULATION] = generer_population_initiale();

	for(size_t i=0; i<nombre_iterations; ++i) {
		size_t selection_parents[NOMBRE_PARENTS] = selectionner_parents(population);

		generer_enfants(&population, selection_parents);
	
		muter_population(&population);
	}
	
	return population[selection_meilleur(population)];
}


