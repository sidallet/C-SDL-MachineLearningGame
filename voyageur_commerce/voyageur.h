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

#define RAYON_POINT 10

int distance_eucli(Point point1,Point point2);
int recherche_local_glouton(int N,Point points[]);

bool verifParcours(const int indicesPointSelect[], const size_t nb_indicesPointSelect, const size_t nombre_noeuds);
