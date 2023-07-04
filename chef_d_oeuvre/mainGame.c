#include "boucle_entrainement_ia.h"
#include "regle.h"
#include "wrapper_sdl.h"
#include <SDL2/SDL2_framerate.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "game.h"

#include "recuit.h"
#include "regle.h"


int main (int argc, char* argv[]) {

	srand(time(NULL));
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect rect_fenetre = {100, 100, 1000, 800};
	if (!start_sdl(&window, &renderer, "Carcheeez", &rect_fenetre)) {
		return EXIT_FAILURE;
	}

	FPSmanager fpsManager;
	SDL_initFramerate(&fpsManager);

	SDL_setFramerate(&fpsManager, 60);

	FILE* fichier = fopen("regleSauv.txt", "r");
	TabRegle tabRecuit = chargerTabRegle(fichier);
	fclose(fichier);


	//boucle_ia(true, tabRegle, &rect_fenetre, renderer, &fpsManager);
	
	//boucle_ia(true, tabRegle, &rect_fenetre, renderer, &fpsManager);
	//boucle_ia(false, tab, &rect_fenetre, renderer, &fpsManager);


	tabRecuit = recuit(500, &rect_fenetre, 104,tabRecuit);

	fichier = fopen("regleSauv.txt", "w");
	afficherTabRegle(fichier,tabRecuit);
	fclose(fichier);

	afficherTabRegle(stdout, tabRecuit);
//	boucle_ia(true, tabRecuit, &rect_fenetre, renderer, &fpsManager);

	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
