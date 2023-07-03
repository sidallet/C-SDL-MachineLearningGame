#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include "TextureHandler.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>

typedef struct
{
	int distance_parcouru;
	int vie;
	int vie_max;
	int delai_invulnerabilite;
	int delai_invulnerabilite_max;
	SDL_Rect voiture;
	TextureHandler textureHandler;
	int deplacement_voiture;
	SDL_Rect* rect_obstacle;
	int nbVoiture;
	int vitesse;

	int ecart_obstacles;
	TTF_Font* font;
} Game;

Game new_game(SDL_Renderer* renderer, SDL_Rect * rect);

void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre);


int deplacer_obstacle(Game* game,SDL_Rect* rect_fenetre, Uint32 deltatime, int distance_parcouru , int nbVoiture);
void game_update(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime);
void liberer_game(Game* game);


void game_afficher(const Game* game, SDL_Renderer* renderer,SDL_Rect* rect_fenetre);
void afficher_obstacle(SDL_Renderer* renderer, const SDL_Rect rect_obstacle[], SDL_Texture *textureObst[], int nombreVoiture);
void afficherRoute(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect_fenetre, int distance_parcourue);
void afficher_texte(SDL_Renderer* renderer,int dist,SDL_Rect* rect_fenetre,int vitesse);
void afficherVie(SDL_Renderer* renderer, SDL_Texture* coeur_rouge, SDL_Texture* coeur_gris, int vie, int vie_max, const SDL_Rect* rect_fenetre);
void afficherFin(SDL_Renderer* renderer, SDL_Rect* rect_fenetre, int score, int vitesse, TTF_Font* font);

//fonction voiture
void deplaceVoiture(SDL_Rect* voiture, int ecart, SDL_Rect* fenetre, int direction_deplacement);
void afficherVoiture(SDL_Renderer * renderer, const SDL_Rect * voiture, SDL_Texture * textureVoiture, int inclinaison, int delai_invulnerabilite);
void voitureAleatoire(Game * game, int pos, SDL_Rect * fenetre);



void afficherEffetDegats(SDL_Renderer* renderer, const int delai_invulnerabilite, const int delai_invulnerabilite_max, const SDL_Rect* rect_fenetre);
bool test_collision(const SDL_Rect* voiture, const SDL_Rect rect_obstacle[], const int nbVoiture);

TTF_Font* charger_font();

#endif //GAME_INCLUDED


