#include "boucle_entrainement_ia.h"
#include "regle.h"
#include "wrapper_sdl.h"
#include <SDL2/SDL2_framerate.h>
#include <stdio.h>
#include <time.h>
#include "game.h"

#include "recuit.h"
#include "regle.h"


int main (int argc, char* argv[]) {

	TabRegle tab = generer_solution_initiale();
	printf("tab regle : i!i!i!i!i!i!ii!!ii!i! \n");
	afficherTabRegle(stdout,tab);
	printf("fin tab regle : i!i!i!i!i!i!ii!!ii!i! \n");

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

	FILE* fichier = fopen("regleTestNul.txt", "r");
	TabRegle tabRegle = chargerTabRegle(fichier);
	afficherTabRegle(stdout, tabRegle);
	fclose(fichier);

	//boucle_ia(true, tabRegle, &rect_fenetre, renderer, &fpsManager);
	//boucle_ia(false, tab, &rect_fenetre, renderer, &fpsManager);
	afficherTabRegle(stdout, recuit(5, &rect_fenetre, 100));


	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
