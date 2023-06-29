#include "wrapper_sdl.h"
#include <SDL2/SDL2_framerate.h>

void affichage(SDL_Renderer* renderer) {

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

}

void deplaceGauche(SDL_Renderer* renderer, SDL_Rect* voiture, SDL_Window* fenetre) {
    int limiteGauche = 0;  //lim gauche de la fenêtre
    int fenetreX, fenetreY;

    SDL_GetWindowPosition(fenetre, &fenetreX, &fenetreY);
    limiteGauche = fenetreX;

    if (voiture->x > limiteGauche) {
        voiture->x -= 1;  //pixel vers la gauche
    }
	//voiture->y += 1;
	//voiture->w += 2;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, voiture);
    SDL_RenderPresent(renderer);
	//voiture->y -= 1;
	//voiture->w -= 2;
}

void deplaceDroite(SDL_Renderer* renderer, SDL_Rect* voiture, SDL_Window* fenetre) {
    int limiteDroite = 0;  //lim gauche de la fenêtre
    int fenetreX, fenetreY;

    SDL_GetWindowPosition(fenetre, &fenetreX, &fenetreY);
    limiteDroite = fenetreX;

    if (voiture->x > limiteDroite) {
        voiture->x -= 1;  //pixel vers la droite
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, voiture);
    SDL_RenderPresent(renderer);
}


int main (int argc, char* argv[]) {

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect rect_fenetre = {100, 100, 1000, 800};
	if (!start_sdl(&window, &renderer, "Carcheeez", &rect_fenetre)) {
		return EXIT_FAILURE;
	}

	FPSmanager fpsManager;
	SDL_initFramerate(&fpsManager);
	SDL_setFramerate(&fpsManager, 60);
	Uint32 delta_time = 0;

	bool actif = true;

	while (actif)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				actif = false;
				break;
			case SDL_KEYDOWN: {
				switch(event.key.keysym.sym)
				{
					case SDLK_q : {
						printf("gauche \n");
						deplaceGauche(renderer,voiture,window);
						break;
					}

					case SDLK_d : {
						printf("droite \n");
						deplaceDroite(renderer,voiture,window);
						break;
					}
				}
			}
			
			default:
				break;
			}
		}
		
		affichage(renderer);

		delta_time = SDL_framerateDelay(&fpsManager);

	}
	

	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
