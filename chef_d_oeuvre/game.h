#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <SDL2/SDL.h>

typedef struct
{
	int distance_parcouru;
	int position_horizontale_voiture;
} Game;

Game new_game();
void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre);
void game_afficher(const Game* game, SDL_Renderer* renderer);


#endif //GAME_INCLUDED


