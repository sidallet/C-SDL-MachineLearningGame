#ifndef VOYAGEUR_INCLUDED
#define VOYAGEUR_INCLUDED

#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include "regle.h"
#include "boucle_entrainement_ia.h"

#define RAYON_POINT 10
#define NUM_THREADS 8

TabRegle generer_solution_initiale();
//void fisherYatesMelange(Chemin* chemin);

//int recuit(Matrice matrice, int N, int nombre_iteration);


#endif //VOYAGEUR_INCLUDED
