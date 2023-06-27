#include <stdio.h>
#include "matrice.h"

#define V 4 // Nombre de sommets du graphe

typedef struct {
    int val;
    int x;
    int y;
}Point;
int distance_eucli(Point point1,Point point2);
void floydWarshall(int graph[V][V]);

