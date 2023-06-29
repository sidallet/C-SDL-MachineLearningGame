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
		.nbVoiture = 6
		
	};
	for (int i = 0; i < game.nbVoiture ; i++)
	{
		voitureAleatoire(&game, i, fenetre);
	}

	printf("voiture obst : \n");
	for (int i = 0; i < game.nbVoiture; i++)
	{
		printf("x %f " , game.rect_obstacle[i].x);
		printf("y %f " , game.rect_obstacle[i].y);
	}
	return game;
}

void voitureAleatoire(Game * game, int pos, SDL_Rect * fenetre)
{
    int xAlea, yAlea;
    bool positionValide = false;
    int diffX, diffY;
    
    while (!positionValide) {
        xAlea = rand() % fenetre->w;
        yAlea = rand() % 400;
        
        positionValide = true;
        
        for (int i = 0; i < pos; i++) {
            diffX = abs(game->rect_obstacle[i].x - xAlea);
            diffY = abs(game->rect_obstacle[i].y + game->rect_obstacle[i].h - yAlea);
            
            if (diffX < 100 && diffY < 100) {
                positionValide = false; 
                break;
            }
        }
    }
    
    SDL_FRect obst = { xAlea, -(yAlea+100), 100.0, 100.0 };
    game->rect_obstacle[pos] = obst;
}


//Appelé une fois par frame
void game_update(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime){
	game->distance_parcouru += deltatime;

	deplacer_obstacle(game,rect_fenetre,deltatime, game->distance_parcouru, game->nbVoiture);

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
	afficher_obstacle(renderer,game->rect_obstacle, game->textureHandler.textures,game->nbVoiture);
	afficherVoiture(renderer,&game->voiture,game->textureHandler.textures[TEXTURE_voiture_course],game->deplacement_voiture*15);
	afficher_texte(renderer, game->distance_parcouru, rect_fenetre);
	afficherVie(renderer, game->textureHandler.textures[TEXTURE_Coeur_rouge], game->textureHandler.textures[TEXTURE_Coeur_gris], game->vie, game->vie_max, rect_fenetre);
}


void afficher_obstacle(SDL_Renderer* renderer, const SDL_FRect rect_obstacle[], SDL_Texture *textureObst[], int nombreVoiture)
{
	SDL_SetRenderDrawColor(renderer, 255,0,0,255);
	for (int i = 0; i < nombreVoiture; i++)
	{
		
		SDL_RenderCopyExF(renderer,textureObst[i%2],NULL,&rect_obstacle[i],0,NULL,SDL_FLIP_VERTICAL);
	}
	
}

void afficherVoiture(SDL_Renderer* renderer, const SDL_Rect* voiture, SDL_Texture* textureVoiture, int inclinaison)
{
	SDL_RenderCopyEx(renderer,textureVoiture,NULL,voiture,inclinaison,NULL,SDL_FLIP_NONE);
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

void deplacer_obstacle(Game* game,SDL_Rect* rect_fenetre, Uint32 deltatime, int distance_parcouru , int nbVoiture){
	int vitesse = 130;
	
	vitesse += distance_parcouru/150;
	if(vitesse > 1500)
	{
		vitesse = 1500;
		
	}
	printf("vitesse %d \n",vitesse);
	for (int i = 0; i < nbVoiture; i++)
	{
		game->rect_obstacle[i].y+=vitesse*deltatime/1000.0;

		if(game->rect_obstacle[i].y > rect_fenetre->h)
		{
			voitureAleatoire(game,i,rect_fenetre);
		}
	}
		
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

