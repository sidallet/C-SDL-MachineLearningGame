#ifndef VOYAGEUR_INCLUDED
#define VOYAGEUR_INCLUDED

#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include "matrice.h"

typedef struct {
    int val;
    int x;
    int y;
}Point;

typedef struct {
	int* val;
	size_t nombre_elems;
} Chemin;

struct params
{
   Matrice dist;
   size_t nombre_points;
   float p;
};


#define RAYON_POINT 10
#define NUM_THREADS 8

int distance_eucli(Point point1,Point point2);
int exist(int list[],int i,const size_t nombre_points);

int Glouton_sans_proba(Matrice dist,const size_t nombre_points);
int Glouton_avec_proba(void * params);


/***
 * @brief Genere les points de manière plutot répartie
 * @param[out] points
 * @param[in] nombre_points
 * @param[in] w_zone
 * @param[in] h_zone
 **/
void generer_points(Point points[], const size_t nombre_points, const Uint32 w_zone, const Uint32 h_zone);
void fisherYatesMelange(Chemin* chemin);

int calculDistanceGraphe(Point * points, const int indicesPointSelect[], const size_t nombre_noeuds);
bool verifParcours(const int indicesPointSelect[], const size_t nb_indicesPointSelect, const size_t nombre_noeuds);

int recuit(Matrice matrice, int N, int nombre_iteration);

Matrice calculMatriceDistance(Matrice adjacence, Point points[], size_t nombre_noeuds);
Matrice floydWarshall(Matrice graph, size_t nombre_noeuds);

bool verifParcours(const int indicesPointSelect[], const size_t nb_indicesPointSelect, const size_t nombre_noeuds);

int calculDistanceGrapheComplet(const Matrice matrice, const Chemin* chemin);

#endif //VOYAGEUR_INCLUDED
