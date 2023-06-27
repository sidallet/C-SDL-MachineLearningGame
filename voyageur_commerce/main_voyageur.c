#include <SDL2/SDL_events.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include <stdbool.h>
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

int main(int argc, char* argv[])
{
    srand(time(NULL));
    int** matrice = (int**)malloc(N * sizeof(int*));
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Rect rect = {0,0,1000,800};
    

    if (!start_sdl(&window,&renderer, "Space Game", &rect))
    {
        return EXIT_FAILURE;
    }

    initMatrice(matrice);
    genereMatriceArbre(matrice, 0, N-1);
    afficheMatrice(matrice);
    libereMatrice(matrice);

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

		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		
		delta_time = SDL_framerateDelay(&fps_manager);
	}

    
    
    clean_sdl(&window,&renderer);

    return 0;
}





