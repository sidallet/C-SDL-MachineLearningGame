#include "boucle_entrainement_ia.h"
#include "game.h"
#include "regle.h"
#include <SDL2/SDL_events.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void affichage(SDL_Renderer* renderer, const Game* game, SDL_Rect* rect_fenetre) {
	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);
	game_afficher(game,renderer, rect_fenetre);
	SDL_RenderPresent(renderer);
}

Observation creerObservation(const Game *game) {
	Observation obs = {
		.routes = {0, 0, 1,0,0}
	};
	return obs;
}

void ia_think(Game* game, const TabRegle* tabRegle) {
	if (game->deplacement_voiture != 0) {
		return; // En déplacement: pas de decision
	}
	Observation observation = creerObservation(game);
	
	Decision decision = choisirRegle(&observation, tabRegle);

	game->deplacement_voiture = decision;
}

int boucle_ia(const bool affichage_actif, TabRegle tabRegle, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager) {
	
	Game game = new_game(renderer, rect_fenetre);
	
	Uint32 delta_time = 0;
	if (!affichage_actif) {
		delta_time = 16;
	}

	while (true) {
		if (affichage_actif) {
			SDL_Event event;
			while(SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					return 0;
				}	
			}
		}

		ia_think(&game, &tabRegle);
		game_update(&game, rect_fenetre, delta_time);
		
		if (game.vie == 0 || game.distance_parcouru >= 100000) {
			return game.distance_parcouru;
		}

		if (affichage_actif) {
			affichage(renderer, &game, rect_fenetre);
			delta_time = SDL_framerateDelay(fpsManager);
		}
	}

	liberer_game(&game);
}



Decision choisirRegle(const Observation* observation, const TabRegle* tabRegle) {
	// Selection
	size_t indice_regles_match[NB_REGLES];
	size_t nb_regles_match = 0;
	for (size_t i=0; i<NB_REGLES; ++i) {
		if (observation_match(*observation, tabRegle->regles[i].observ)) {
			indice_regles_match[nb_regles_match] = i;
			nb_regles_match++;
		}	
	}

	// Tirage
	int cumul_priorite = 0;
	printf("Obs \n");
	for (size_t i=0; i<nb_regles_match; ++i) {
		afficherObservation(stdout, tabRegle->regles[indice_regles_match[i]].observ);
		printf("\n");
		cumul_priorite += tabRegle->regles[indice_regles_match[i]].priorite;
	}

	float alea = ((float)rand()) / RAND_MAX;
	for (size_t i=0; i<nb_regles_match; ++i) {
		float probaI = ((float)tabRegle->regles[indice_regles_match[i]].priorite) / cumul_priorite;
		if (alea < probaI) {
			return tabRegle->regles[indice_regles_match[i]].decis;
		}
		alea -= probaI;
	}

	return RIEN;
}

bool observation_match(const Observation obs1, const Observation obs2) {
	for (size_t i=0; i<5; ++i) {
		if (obs1.routes[i] == JOKER || obs2.routes[i] == JOKER) {
			continue;
		}
		if (obs1.routes[i] != obs2.routes[i]) {
			return false;
		}
	}

	return true;
}
