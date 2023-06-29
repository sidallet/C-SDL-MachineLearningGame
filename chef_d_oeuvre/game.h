#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <SDL2/SDL.h>
#include "TextureHandler.h"
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct
{
	int distance_parcouru;
	SDL_Rect voiture;
	TextureHandler textureHandler;
} Game;

Game new_game();
void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre);
void game_afficher(const Game* game, SDL_Renderer* renderer);
void liberer_game(Game* game);

//fonction voiture
void deplaceGauche(SDL_Rect* voiture, SDL_Rect* fenetre);
void deplaceDroite(SDL_Rect* voiture, SDL_Rect* fenetre);
void afficherVoiture(SDL_Renderer * renderer, const SDL_Rect * voiture, SDL_Texture * textureVoiture);

#endif //GAME_INCLUDED


