#include <stdio.h>
typedef struct {
    int val;
    int x;
    int y;
}Point;

#define RAYON_POINT 10

int distance_eucli(Point point1,Point point2);