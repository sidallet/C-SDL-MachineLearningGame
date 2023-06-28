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


#define RAYON_POINT 10

int distance_eucli(Point point1,Point point2);
int recherche_local_glouton(Matrice matrice,int N,Point points[]);

int recuit(Matrice matrice, int N, int nombre_iteration);


int calculDistanceGraphe(Point * points, const int indicesPointSelect[], const size_t nombre_noeuds);




bool verifParcours(const int indicesPointSelect[], const size_t nb_indicesPointSelect, const size_t nombre_noeuds);
