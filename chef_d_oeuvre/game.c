#include "game.h"
#define RAYON_POINT 10

Game new_game(SDL_Renderer* renderer) {
	Game game = {
		.distance_parcouru = 0,
		.voiture = {0,0,400,400},
		.textureHandler = newTextureHandler(renderer)
	};
	return game;
}

void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre) {

	switch (event->type)
	{
		case SDL_KEYDOWN: {
			switch(event->key.keysym.sym)
			{
				case SDLK_q : {
					printf("gauche \n");
					deplaceGauche(&game->voiture,rect_fenetre);
					break;
				}

				case SDLK_d : {
					printf("droite \n");
					deplaceDroite(&game->voiture,rect_fenetre);
					break;
				}
			}
		}
	default:
		break;
	}
}

void game_afficher(const Game* game, SDL_Renderer* renderer) {
	afficherVoiture(renderer,&game->voiture,game->textureHandler.textures[TEXTURE_voiture_course]);
}

void afficher_texte(SDL_Renderer* renderer,int dist,SDL_Rect* rect_fenetre){
	char dist_char[10];
	sprintf(dist_char, "%d", dist);
	stringRGBA(renderer, rect_fenetre->w*0.2,rect_fenetre->h*0.2, dist_char, 0, 0, 20, 255);  //affichage texte
}
void liberer_game(Game* game) {
	freeTextureHandler(&game->textureHandler);
}

void deplaceGauche(SDL_Rect* voiture, SDL_Rect* fenetre) {
    int limiteGauche = 0;  //lim gauche de la fenêtre

    if (voiture->x > limiteGauche) {
        voiture->x -= 1;  //pixel vers la gauche
    }
	//voiture->y += 1;
	//voiture->w += 2;

	//voiture->y -= 1;
	//voiture->w -= 2;
}

void deplaceDroite(SDL_Rect* voiture, SDL_Rect* fenetre) {
    int limiteDroite = fenetre->w;  //lim gauche de la fenêtre

    if (voiture->x > limiteDroite) {
        voiture->x -= 1;  //pixel vers la droite
    }

    
}

void afficherVoiture(SDL_Renderer * renderer, const SDL_Rect * voiture, SDL_Texture * textureVoiture)
{
	printf("%p \n", textureVoiture);
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	//SDL_RenderCopyEx(renderer,textureVoiture,NULL,voiture,0,NULL,SDL_FLIP_NONE);
	SDL_RenderCopy(renderer,textureVoiture,NULL,voiture);
	SDL_RenderFillRect(renderer,voiture);
    SDL_RenderFillRect(renderer, voiture);
}
