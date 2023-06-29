#include "game.h"
#define RAYON_POINT 10

Game new_game(SDL_Renderer* renderer) {
	Game game = {
		.distance_parcouru = 0,
		.position_horizontale_voiture = 0,
		.textureHandler = newTextureHandler(renderer),
		.rect_obstacle = {100.0,0.0,100.0,100.0},        //coordonnée x, coordonée y, taille x, taille y
	};
	return game;
}
void game_update(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime){
	deplacer_obstacle(game,rect_fenetre,deltatime);
}

void deplacer_obstacle(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime){
	game->rect_obstacle.y+=200.0*deltatime/1000.0;
}




void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre) {

	switch (event->type)
	{
	default:
		break;
	}
}

void game_afficher(const Game* game, SDL_Renderer* renderer) {

	afficher_obstacle(renderer,&game->rect_obstacle);
}


void afficher_obstacle(SDL_Renderer* renderer, const SDL_FRect* rect_obstacle)
{
	SDL_SetRenderDrawColor(renderer, 255,0,0,255);
	SDL_RenderFillRectF(renderer, rect_obstacle);
}
void afficher_texte(SDL_Renderer* renderer,int dist,SDL_Rect* rect_fenetre){
	char dist_char[10];
	sprintf(dist_char, "%d", dist);
	stringRGBA(renderer, rect_fenetre->w*0.2,rect_fenetre->h*0.2, dist_char, 0, 0, 20, 255);  //affichage texte
}
void liberer_game(Game* game) {
	freeTextureHandler(&game->textureHandler);
}
