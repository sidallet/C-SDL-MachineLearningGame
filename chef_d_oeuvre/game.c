#include "game.h"

Game new_game() {
	Game game = {
		.distance_parcouru = 0,
		.position_horizontale_voiture = 0
	};
	return game;
}

void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre) {

	switch (event->type)
	{
	default:
		break;
	}
}

void game_afficher(const Game* game, SDL_Renderer* renderer) {
	
}
