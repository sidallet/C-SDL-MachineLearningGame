#include "voyageur.h"
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_rect.h>

int distance_eucli(Point point1,Point point2)
{
    int dist;
    dist =sqrt((point1.x-point2.x)*(point1.x-point2.x)+(point1.y-point2.y)*(point1.y-point2.y));
    return dist;
}

SDL_Rect create_rect_cadre(const size_t num_cadre, const size_t nb_colones, const size_t nb_lignes, const Uint32 w_zone, const Uint32 h_zone) {
	size_t ligne = num_cadre/nb_colones;
	size_t colone = num_cadre%nb_colones;
	SDL_Rect rect;
	rect.w = w_zone/nb_lignes; 
	rect.h = h_zone/nb_colones; 
	rect.x = ligne*rect.w;
	rect.y = colone*rect.h;
	return rect;
}

void generer_points(Point points[], const size_t nombre_points, const Uint32 w_zone, const Uint32 h_zone) {
	const size_t nombre_cadres = 9*nombre_points;
	size_t nb_colones = floor(sqrt((double)nombre_cadres));
	size_t nb_lignes = (nombre_cadres / nb_colones)+1;
	bool cases[nombre_cadres];

	for (size_t i=0; i<nombre_points; ++i) {
		size_t num_cadre = rand()%nombre_cadres;
		while (!cases[num_cadre]) {
			num_cadre++;
			if (num_cadre == nombre_cadres) {
				num_cadre = 0;
			}
		}
		
		SDL_Rect rect_cadre = create_rect_cadre(num_cadre, nb_colones, nb_lignes, w_zone, h_zone);
		points[i].x = rect_cadre.x + rand()%rect_cadre.w;
		points[i].y = rect_cadre.y + rand()%rect_cadre.h;
		points[i].val = i;
	
		// Ligne precedente
		if (num_cadre >= nb_colones) {
			const size_t rang = num_cadre-nb_colones;
			cases[rang] = false;
			if (rang%nb_colones!=0) {
				cases[rang -1] = false;
			}
			if ((rang+1)%nb_colones!=0) {
				cases[rang+1] = false;
			}
		}
		// Ligne actuelle
		cases[num_cadre] = false;
		if (num_cadre%nb_colones!=0) {
			cases[num_cadre-1] = false;
		}
		if ((num_cadre+1)%nb_colones!=0) {
			cases[num_cadre+1] = false;
		}
		// Ligne suivante
		if (nombre_cadres - num_cadre >= nb_colones) {
			const size_t rang = num_cadre+nb_colones;

			cases[rang] = false;
			if (rang%nb_colones!=0) {
				cases[rang -1] = false;
			}
			if ((rang+1)%nb_colones!=0) {
				cases[rang+1] = false;
			}
		}
	}
}

bool verifParcours(const int indicesPointSelect[], const size_t nb_indicesPointSelect, const size_t nombre_noeuds)
{
	if (!(nb_indicesPointSelect > 0 && indicesPointSelect[0] == indicesPointSelect[nb_indicesPointSelect - 1]))
	{
		return false;
	}

	for (size_t i = 0; i < nombre_noeuds; i++)
	{
		bool pointPresent = false;
		for (size_t j = 0; j < nb_indicesPointSelect; j++)
		{
			if (indicesPointSelect[j] == i)
			{
				pointPresent = true;
			}
		}
		if (!pointPresent)
		{
			return false;
		}
	}
	return true;
}

int calculDistanceGraphe(Point * points, const int indicesPointSelect[], const size_t nombre_noeuds)
{
	int distTotal = 0;
	for (size_t i = 0; i+1 < nombre_noeuds; i++)
	{
		distTotal += distance_eucli(points[indicesPointSelect[i]], points[indicesPointSelect[i+1]]);
	}
	return distTotal;
}
