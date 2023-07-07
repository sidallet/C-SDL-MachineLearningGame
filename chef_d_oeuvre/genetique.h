#ifndef GENETIQUE_H_INCLUDED
#define GENETIQUE_H_INCLUDED

#include "regle.h"
#include <SDL2/SDL_rect.h>

#define NOMBRE_PARENTS 100
#define TAILLE_POPULATION 1000

TabRegle genetique(int nombre_iterations, SDL_Rect * rect_fenetre, size_t nb_parties, TabRegle population_initiale[TAILLE_POPULATION]);


/**
 * @brief Genere la population initiale
 * @param [out] population
 * 
**/
void generer_population_initiale(TabRegle population[]);

/**
 * @param [in, out] population
**/
void selectionner_parents(TabRegle population[], int scores[]);


#endif //GENETIQUE_H_INCLUDED

