#include "boucle_entrainement_ia.h"
#include "regle.h"
#include "wrapper_sdl.h"
#include <SDL2/SDL2_framerate.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "game.h"


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

	FILE* fichier = fopen("regleTest.txt", "r");
	TabRegle tabRegle = chargerTabRegle(fichier);
	fclose(fichier);
	
	time_t dep = clock();
	for (size_t i=0; i<1000; ++i) {
		boucle_ia(false, tabRegle, &rect_fenetre, renderer, &fpsManager);
	}
	printf("%f\n", (clock()-dep)*1000.0/CLOCKS_PER_SEC);


	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
