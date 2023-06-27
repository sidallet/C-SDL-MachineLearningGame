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

void afficherGraphe(SDL_Renderer* renderer, int** matrice_graphe, const Point noeuds[], const size_t nombre_noeuds) {
	for (size_t j = 0; j<nombre_noeuds; ++j) {
		for (size_t i = j; i<nombre_noeuds; ++i) {
			if (matrice_graphe[i][j]) {
				Uint16 hash_red = noeuds[i].x ^ noeuds[j].y*7;
				thickLineRGBA(renderer, noeuds[i].x, noeuds[i].y, noeuds[j].x, noeuds[j].y, 3, hash_red, j*10, 255, 255);
			}
		}
	}

	for (size_t i=0; i<nombre_noeuds; ++i) {
		filledCircleRGBA(renderer, noeuds[i].x, noeuds[i].y, 10, 25*i,255,0,255);
		characterRGBA(renderer, noeuds[i].x-2, noeuds[i].y-2, toascii(noeuds[i].val+'0'), 0, 0, 30, 255);
	} 
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    int** matrice = (int**)malloc(N * sizeof(int*));
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Rect rect_fenetre = {0,0,1000,800};
    

    if (!start_sdl(&window,&renderer, "Space Game", &rect_fenetre))
    {
        return EXIT_FAILURE;
    }

	Point points[N];
	
	for (size_t i=0; i<N; ++i) {
		points[i].val = i;
		points[i].x = rand()%rect_fenetre.w;
		points[i].y = rand()%rect_fenetre.h;
		printf("%d, %d\n", points[i].x, points[i].y);
	}

    initMatrice(matrice);
    genereMatriceArbre(matrice, 0, N-1);
    afficheMatrice(matrice);


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
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		afficherGraphe(renderer, matrice, points, N);
		SDL_RenderPresent(renderer);
		
		delta_time = SDL_framerateDelay(&fps_manager);
	}

    
    
    libereMatrice(matrice);
    clean_sdl(&window,&renderer);

    return 0;
}





