#include "game.h"

Game new_game(SDL_Renderer* renderer) {
	Game game = {
		.distance_parcouru = 0,
		.position_horizontale_voiture = 0,
		.textureHandler = newTextureHandler(renderer)
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

// void afficher_texte()
// stringRGBA(renderer, noeuds[i].x-RAYON_POINT/4, noeuds[i].y-RAYON_POINT/4, nom_noeud, 0, 0, 20, 255);  //affichage texte
void liberer_game(Game* game) {
	freeTextureHandler(&game->textureHandler);
}
