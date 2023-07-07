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
				if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
					game_handle_event(&game,&event);
				}
				if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d) {
					game_handle_event(&game,&event);
				}
			}
		}
		const int colVoiture = game.voiture.x/(game.voiture.w+game.ecart_obstacles);
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

	// FILE* fichier = fopen("regleSauv.txt", "r");
	// TabRegle tabRecuit = chargerTabRegle(fichier);
	// fclose(fichier);


	//boucle_ia(true, tabRegle, &rect_fenetre, renderer, &fpsManager);
	
	//boucle_ia(true, tabRegle, &rect_fenetre, renderer, &fpsManager);
	//boucle_ia(false, tab, &rect_fenetre, renderer, &fpsManager);


	// tabRecuit = recuit(2000, &rect_fenetre, 8,tabRecuit);

	// fichier = fopen("regleSauv.txt", "w");
	// afficherTabRegle(fichier,tabRecuit);
	// fclose(fichier);

	// afficherTabRegle(stdout, tabRecuit);
	 //boucle_ia(true, tabRecuit, &rect_fenetre, renderer, &fpsManager);
	int scoreJoueur = boucle_joueur(true, &rect_fenetre, renderer, &fpsManager);
	printf("[Score] -> %d\n",scoreJoueur);
	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
#endif
