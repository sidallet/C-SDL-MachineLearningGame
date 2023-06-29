#include "game.h"
#define RAYON_POINT 10

Game new_game(SDL_Renderer* renderer, SDL_Rect * fenetre) {
	printf("w et h %d %d \n", fenetre->w, fenetre->h);
	Game game = {
		.distance_parcouru = 0,
		.voiture = {fenetre->w/2 - 100,fenetre->h-125,100,100},
		.textureHandler = newTextureHandler(renderer),
		.inclinaison = 0
	};
	return game;
}

void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre, Uint32 delta_time) {

	switch (event->type)
	{
		case SDL_KEYDOWN: {
			switch(event->key.keysym.sym)
			{
				case SDLK_q : {
					printf("gauche \n");
					deplaceGauche(&game->voiture,rect_fenetre, &game->inclinaison);
					break;
				}

				case SDLK_d : {
					printf("droite \n");
					deplaceDroite(&game->voiture,rect_fenetre, &game->inclinaison);
					break;
				}
			}
		}
	default:
		break;
	}
}

void game_afficher(const Game* game, SDL_Renderer* renderer) {
	afficherVoiture(renderer,&game->voiture,game->textureHandler.textures[TEXTURE_voiture_course],game->inclinaison);
}

void afficher_texte(SDL_Renderer* renderer,int dist,SDL_Rect* rect_fenetre){
	char dist_char[10];
	sprintf(dist_char, "%d", dist);
	stringRGBA(renderer, rect_fenetre->w*0.2,rect_fenetre->h*0.2, dist_char, 0, 0, 20, 255);  //affichage texte
}
void liberer_game(Game* game) {
	freeTextureHandler(&game->textureHandler);
}

void deplaceGauche(SDL_Rect* voiture, SDL_Rect* fenetre, int *inclinaison) {
    int limiteGauche = 0;  //lim gauche de la fenêtre
	*inclinaison =-15;
    if (voiture->x > limiteGauche) {
        voiture->x -= 10;  //pixel vers la gauche
    }
	//voiture->y += 1;
	//voiture->w += 2;

	//voiture->y -= 1;
	//voiture->w -= 2;
}

void deplaceDroite(SDL_Rect* voiture, SDL_Rect* fenetre, int *inclinaison) {
    int limiteDroite = fenetre->w;  //lim gauche de la fenêtre
	*inclinaison = 15;

	printf("voiture %d limiteDroite %d\n", voiture->x, limiteDroite);
    if (voiture->x+100 < limiteDroite) {
        voiture->x += 10;  //pixel vers la droite
    }

    
}


void afficherVoiture(SDL_Renderer* renderer, const SDL_Rect* voiture, SDL_Texture* textureVoiture, int inclinaison)
{
	printf("incli : %d \n", inclinaison);
	SDL_RenderCopyEx(renderer,textureVoiture,NULL,voiture,inclinaison,NULL,SDL_FLIP_NONE);
}


