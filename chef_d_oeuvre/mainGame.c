#include "boucle_entrainement_ia.h"
#include "regle.h"
#include "wrapper_sdl.h"
#include <SDL2/SDL2_framerate.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

#include "recuit.h"
#include "regle.h"


#ifndef UNIT_TEST
int main (int argc, char* argv[]) {
	char nom_fichier_regle[300] = "regleSauv.txt";
	if (argc >= 2) {
		strncpy(nom_fichier_regle, argv[1], 299);
	}


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

	FILE* fichier = fopen(nom_fichier_regle, "r");
	if (fichier==NULL) {
		fprintf(stderr, "Erreur ouverture fichier regle %s\n", nom_fichier_regle);
		return EXIT_FAILURE;
	}
	TabRegle tabRecuit = chargerTabRegle(fichier);
	fclose(fichier);

	afficherTabRegle(stdout, tabRecuit);
	int scoremax =boucle_ia(true, tabRecuit, &rect_fenetre, renderer, &fpsManager);
	printf("Score IA :%d\n",scoremax/175);

	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
#endif
