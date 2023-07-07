#include "boucle_entrainement_ia.h"
#include "regle.h"
#include "wrapper_sdl.h"
#include <SDL2/SDL2_framerate.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "game.h"

#include "regle.h"
#include "genetique.h"
#include "recuit.h"

#ifndef UNIT_TEST
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

	//boucle_ia(true, tabRegle, &rect_fenetre, renderer, &fpsManager);
	
	//boucle_ia(true, tabRegle, &rect_fenetre, renderer, &fpsManager);
	//boucle_ia(false, tab, &rect_fenetre, renderer, &fpsManager);


	FILE* file = fopen("regleSauv.txt", "r");
	TabRegle rBase = chargerTabRegle(file);
	fclose(file);

	TabRegle population[TAILLE_POPULATION];
	for (size_t i=0; i<TAILLE_POPULATION; ++i) {
		population[i] = copier_tab_regle(recuit(30, &rect_fenetre, 8, rBase));
	}

	TabRegle tabRecuit = genetique(100, &rect_fenetre, 8*3, population);


	afficherTabRegle(stdout, tabRecuit);
	boucle_ia(true, tabRecuit, &rect_fenetre, renderer, &fpsManager);

	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
#endif
