#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <SDL2/SDL.h>
#include "TextureHandler.h"
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct
{
	int distance_parcouru;
	int position_horizontale_voiture;
	TextureHandler textureHandler;
} Game;

Game new_game();
void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre);
void game_afficher(const Game* game, SDL_Renderer* renderer);


#endif //GAME_INCLUDED


