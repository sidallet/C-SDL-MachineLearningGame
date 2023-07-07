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

int boucle_joueur(const bool affichage_actif, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager) {
	Game game = new_game(affichage_actif, renderer, rect_fenetre);
		
	Uint32 delta_time = 0;
	if (!affichage_actif) {
		delta_time = 16;
	}
	else {
		SDL_initFramerate(fpsManager);
		SDL_setFramerate(fpsManager, 60);
	}
	bool vision = false;
	bool running = true;
	while (running) {
		if (affichage_actif) {
			SDL_Event event;
			while(SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					running = false;
				}	
				game_handle_event(&game,&event);
			}
		}
		//const int colVoiture = game.voiture.x/(game.voiture.w+game.ecart_obstacles);
		game_update(&game, rect_fenetre, delta_time);
		
		if (game.vie == 0 || game.distance_parcouru >= 1000000) {
			running = false;
		}

		if (affichage_actif) {
			affichageJoueur(renderer, &game, rect_fenetre);
			delta_time = SDL_framerateDelay(fpsManager);
		}
	}

	const int score = calculerScore(&game);
	liberer_game(&game);
	return score;
}


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
	TabRegle tabRecuit = chargerTabRegle(file);
	fclose(file);

	tabRecuit = recuit(20000, &rect_fenetre, 8,tabRecuit);
	FILE* fileo = fopen("regleSauv.txt", "w");
	afficherTabRegle(fileo, tabRecuit);
	fclose(fileo);



	afficherTabRegle(stdout, tabRecuit);
	boucle_ia(true, tabRecuit, &rect_fenetre, renderer, &fpsManager);
	//int scoreJoueur = boucle_joueur(true, &rect_fenetre, renderer, &fpsManager);
	//printf("[Score] -> %d\n",scoreJoueur/175);
	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
#endif
