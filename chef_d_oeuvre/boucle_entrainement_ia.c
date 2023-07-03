#include "boucle_entrainement_ia.h"
#include <stdbool.h>


void affichage(SDL_Renderer* renderer, const Game* game, SDL_Rect* rect_fenetre) {
	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);
	game_afficher(game,renderer, rect_fenetre);
	SDL_RenderPresent(renderer);
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

}


