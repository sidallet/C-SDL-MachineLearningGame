#include "game.h"
#include "TextureHandler.h"
#include <SDL2/SDL_render.h>
#define RAYON_POINT 10

Game new_game(SDL_Renderer* renderer, SDL_Rect * fenetre) {
	printf("w et h %d %d \n", fenetre->w, fenetre->h);
	Game game = {
		.distance_parcouru = 0,
		.vie = 3,
		.vie_max = 5,
		.voiture = {fenetre->w/2 - 100,fenetre->h-125,100,100},
		.textureHandler = newTextureHandler(renderer),
		.deplacement_voiture = 0,
		.rect_obstacle = {100.0,-200.0,100.0,100.0}        //coordonnée x, coordonée y, taille x, taille y
		
	};
	return game;
}

//Appelé une fois par frame
void game_update(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime){
	game->distance_parcouru += deltatime;

	deplacer_obstacle(game,rect_fenetre,deltatime);

	deplaceVoiture(&game->voiture,rect_fenetre, game->deplacement_voiture, deltatime);


	//Gerer collision ici
}


void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre) {

	switch (event->type)
	{
		case SDL_KEYDOWN: {
			switch(event->key.keysym.sym)
			{
				case SDLK_LEFT:
				case SDLK_q : {
					game->deplacement_voiture=-1;
					break;
				}

				case SDLK_RIGHT:
				case SDLK_d : {
					game->deplacement_voiture=1;
					break;
				}
			}
			break;
		}
		case SDL_KEYUP: {
			game->deplacement_voiture = 0;
			break;
		}
	default:
		break;
	}


}

void game_afficher(const Game* game, SDL_Renderer* renderer, SDL_Rect* rect_fenetre) {
	afficherRoute(renderer, game->textureHandler.textures[TEXTURE_Route], rect_fenetre, game->distance_parcouru);
	afficher_obstacle(renderer,&game->rect_obstacle);
	afficherVoiture(renderer,&game->voiture,game->textureHandler.textures[TEXTURE_voiture_course],game->deplacement_voiture*15);
	afficher_texte(renderer, game->distance_parcouru, rect_fenetre);
	afficherVie(renderer, game->textureHandler.textures[TEXTURE_Coeur_rouge], game->textureHandler.textures[TEXTURE_Coeur_gris], game->vie, game->vie_max, rect_fenetre);
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

void deplaceVoiture(SDL_Rect* voiture, SDL_Rect* fenetre, int direction_deplacement, Uint32 delta_time) {
    int limiteGauche = 0;  //lim gauche de la fenêtre
    int limiteDroite = fenetre->w;  //lim gauche de la fenêtre
	float vitesse = 300.0;
    voiture->x += direction_deplacement * vitesse * delta_time / 1000.0;
	if (voiture->x < limiteGauche) {
        voiture->x = limiteGauche;  //pixel vers la gauche
    }
	if (voiture->x+voiture->w > limiteDroite) {
		voiture->x = limiteDroite - voiture->w;
	}
}

void deplacer_obstacle(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime){
	game->rect_obstacle.y+=200.0*deltatime/1000.0;
}

void afficherVoiture(SDL_Renderer* renderer, const SDL_Rect* voiture, SDL_Texture* textureVoiture, int inclinaison)
{
	SDL_RenderCopyEx(renderer,textureVoiture,NULL,voiture,inclinaison,NULL,SDL_FLIP_NONE);
}

void afficherRoute(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect_fenetre, int distance_parcourue) {
	SDL_Rect rect_dest = {
		.x = 0,
		.y = distance_parcourue%rect_fenetre->h,
		.w = rect_fenetre->w,
		.h = rect_fenetre->h
	};
	SDL_RenderCopy(renderer, texture, NULL, &rect_dest);
	
	rect_dest.y -= rect_fenetre->h;
	SDL_RenderCopy(renderer, texture, NULL, &rect_dest);
}

void afficherVie(SDL_Renderer* renderer, SDL_Texture* coeur_rouge, SDL_Texture* coeur_gris, int vie, int vie_max, const SDL_Rect* rect_fenetre) {
	int espace_entre_coeur = 20;
	int taille_coeur = 50;
	SDL_Rect rect_dest = {
		.x = rect_fenetre->w - vie_max * (taille_coeur + espace_entre_coeur),
		.y = 20,
		.w = taille_coeur,
		.h = taille_coeur
	};	

	for (int i=0; i<vie; ++i) {
		SDL_RenderCopy(renderer, coeur_rouge, NULL, &rect_dest);
		rect_dest.x += taille_coeur + espace_entre_coeur;
	}
	for (int i=0; i<vie_max - vie; ++i) {
		SDL_RenderCopy(renderer, coeur_gris, NULL, &rect_dest);
		rect_dest.x += taille_coeur + espace_entre_coeur;
	}

}

