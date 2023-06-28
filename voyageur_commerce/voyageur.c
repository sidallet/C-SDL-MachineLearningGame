#include "voyageur.h"
#include <math.h>
#include <stdbool.h>

int distance_eucli(Point point1,Point point2)
{
    int dist;
    dist =sqrt((point1.x-point2.x)*(point1.x-point2.x)+(point1.y-point2.y)*(point1.y-point2.y));
    return dist;
}


void generer_points(Point points[], const size_t nombre_points, const Uint32 w_zone, const Uint32 h_zone) {
	bool cases[12*nombre_points];

}

