#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
typedef struct {
    int val;
    int x;
    int y;
}Point;
#define RAYON_POINT 10

int distance_eucli(Point point1,Point point2);

/***
 * @brief Genere les points de manière plutot répartie
 * @param[out] points
 * @param[in] nombre_points
 * @param[in] w_zone
 * @param[in] h_zone
 **/
void generer_points(Point points[], const size_t nombre_points, const Uint32 w_zone, const Uint32 h_zone);

