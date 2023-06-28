#include <SDL2/SDL_events.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper_sdl.h"
#include "voyageur.h"
#include "matrice.h"


void draw_rec(SDL_Renderer* renderer,int rectWidth, int rectHeight,int rectx,int recty,int Red,int Green,int Blue)
{  
    SDL_Rect rect;
    rect.x = rectx;
    rect.y = recty;
    rect.w = rectWidth;
    rect.h = rectHeight;
    SDL_SetRenderDrawColor(renderer,Red,Green,Blue,200);
    SDL_RenderFillRect(renderer, &rect);

}

void afficherGraphe(SDL_Renderer* renderer, int** matrice_graphe, const Point noeuds[], const size_t nombre_noeuds, int indicesPointSelect[], int nb_indicesPointSelect) {
	for (size_t j = 0; j<nombre_noeuds; ++j) {
		for (size_t i = j; i<nombre_noeuds; ++i) {
			if (matrice_graphe[i][j]) {
				Uint16 hash_red = noeuds[i].x ^ noeuds[j].y*7;
				thickLineRGBA(renderer, noeuds[i].x, noeuds[i].y, noeuds[j].x, noeuds[j].y, 3, hash_red, j*10, 255, 255);
			}
		}
	}
	//Affichage points selectionnés
	for (size_t i=0; i+1<nb_indicesPointSelect; ++i) {
		circleRGBA(renderer, noeuds[indicesPointSelect[i]].x, noeuds[indicesPointSelect[i]].y, RAYON_POINT*1.3, 255,255,255,255);
		thickLineRGBA(renderer, noeuds[indicesPointSelect[i]].x, noeuds[indicesPointSelect[i]].y, noeuds[indicesPointSelect[i+1]].x, noeuds[indicesPointSelect[i+1]].y, 3, 255, 255, 255, 255);
	} 

	//Affichage des points
	for (size_t i=0; i<nombre_noeuds; ++i) {
		filledCircleRGBA(renderer, noeuds[i].x, noeuds[i].y, RAYON_POINT, 25*i,255,0,255);
		characterRGBA(renderer, noeuds[i].x-2, noeuds[i].y-2, toascii(noeuds[i].val+'0'), 0, 0, 30, 255);
	} 

	//Affichage du dernier point selectionné
	if(nb_indicesPointSelect > 0)
	{
		filledCircleRGBA(renderer, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].x, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].y, RAYON_POINT*1.1, 255,255,255,255);
		characterRGBA(renderer, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].x-2, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].y-2, toascii(noeuds[indicesPointSelect[nb_indicesPointSelect-1]].val+'0'), 0, 0, 30, 255);
	}

	//Affichage du numero des points
	for (size_t i=0; i<nombre_noeuds; ++i) {
		characterRGBA(renderer, noeuds[i].x-2, noeuds[i].y-2, toascii(noeuds[i].val+'0'), 0, 0, 30, 255);
	} 
}

int cherchePointClick(const Point noeuds[], const size_t nombre_noeuds, int x_mouse, int y_mouse)
{
	Point click = {
		.x = x_mouse,
		.y = y_mouse
	};
	for (size_t i = 0; i < nombre_noeuds; i++)
	{
		if(distance_eucli(noeuds[i],click) < RAYON_POINT )
		{
			return i;
		}
	}
	return -1;
	
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


int main(int argc, char* argv[])
{
    float p = 0.1;
	int x_mouse, y_mouse, point_click;
	int distTotal;
    srand(time(NULL));

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Rect rect_fenetre = {0,0,1000,800};
    

    if (!start_sdl(&window,&renderer, "Space Game", &rect_fenetre))
    {
        return EXIT_FAILURE;
    }

	const size_t nombre_points = 5;
	Matrice matrice = initMatrice(nombre_points);
    genereMatriceArbre(matrice, 0, nombre_points-1);
    genereGraphe(matrice,p, nombre_points);

	Point points[nombre_points];

	int indicesPointSelect[nombre_points*4];
	int nb_indicesPointSelect = 0;
	
	for (size_t i=0; i<nombre_points; ++i) {
		points[i].val = i;
		points[i].x = rand()%rect_fenetre.w;
		points[i].y = rand()%rect_fenetre.h;
		printf("%d, %d\n", points[i].x, points[i].y);
	}
	FPSmanager fps_manager;
	SDL_initFramerate(&fps_manager);
	SDL_setFramerate(&fps_manager, 60);

	Uint32 delta_time = 0;

	bool actif = true;
	while (actif) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					actif = false;
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					switch(event.button.clicks)
					{
					case SDL_BUTTON_LEFT: {
						x_mouse = event.button.x;
                        y_mouse = event.button.y;
						point_click = cherchePointClick(points, nombre_points, x_mouse, y_mouse);
						if(point_click != -1)
						{
							printf("point click %d \n", point_click);
							if(nb_indicesPointSelect == 0 || matrice[point_click][indicesPointSelect[nb_indicesPointSelect-1]] == 1)
							{
								indicesPointSelect[nb_indicesPointSelect] = point_click;
								nb_indicesPointSelect++;

							}
							else if (nb_indicesPointSelect>0 && point_click == indicesPointSelect[nb_indicesPointSelect-1]){
								nb_indicesPointSelect--;
							}
							bool parcoursOK = verifParcours(indicesPointSelect, nb_indicesPointSelect, nombre_points);
							printf("parcours %d \n", parcoursOK);

							if(parcoursOK)
							{

								distTotal = calculDistanceGraphe(points, indicesPointSelect,nb_indicesPointSelect);
								printf("Dist total = %d\n",distTotal);
							}
						}
						break;
					}
					default:
						break;
					} 
				}

			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		afficherGraphe(renderer, matrice, points, nombre_points, indicesPointSelect,nb_indicesPointSelect);
		SDL_RenderPresent(renderer);
		
		delta_time = SDL_framerateDelay(&fps_manager);
	}

    
    
    libereMatrice(matrice, nombre_points);
    clean_sdl(&window,&renderer);

    return 0;
}





