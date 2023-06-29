#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <SDL2/SDL.h>
#include "TextureHandler.h"
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct
{
	int distance_parcouru;
	int vie;
	int vie_max;
	SDL_Rect voiture;
	TextureHandler textureHandler;
	int deplacement_voiture;
	SDL_FRect  rect_obstacle;
} Game;

Game new_game(SDL_Renderer* renderer, SDL_Rect * rect);

void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre);


void deplacer_obstacle(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime);
void game_update(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime);
void liberer_game(Game* game);


void game_afficher(const Game* game, SDL_Renderer* renderer,SDL_Rect* rect_fenetre);
void afficher_obstacle(SDL_Renderer* renderer,const SDL_FRect* rect_obstacle);
void afficherRoute(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect_fenetre, int distance_parcourue);
void afficher_texte(SDL_Renderer* renderer,int dist,SDL_Rect* rect_fenetre);
void afficherVie(SDL_Renderer* renderer, SDL_Texture* coeur_rouge, SDL_Texture* coeur_gris, int vie, int vie_max, const SDL_Rect* rect_fenetre);

//fonction voiture
void deplaceVoiture(SDL_Rect* voiture, SDL_Rect* fenetre, int direction_deplacement, Uint32 delta_time);
void afficherVoiture(SDL_Renderer * renderer, const SDL_Rect * voiture, SDL_Texture * textureVoiture, int inclinaison);

#endif //GAME_INCLUDED


