#include "game.h"
#include "TextureHandler.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#define RAYON_POINT 10

Game new_game(bool affichage_on, SDL_Renderer* renderer, SDL_Rect * fenetre) {
	Game game = {
		.distance_parcouru = 0,
		.vie = 5,
		.vie_max = 5,
		.delai_invulnerabilite = -1,
		.delai_invulnerabilite_max = 800,
		.voiture = {0,fenetre->h-125,65,110},
		.deplacement_voiture = 0, 
		.nbVoiture = 10,
		.vitesse = 100,
		.ecart_obstacles = 15,
		.temps_deplacement = 0,
	};

	if (affichage_on) {
		game.textureHandler = newTextureHandler(renderer);
		game.font = charger_font();
	}
	else {
		game.font = NULL;
	}

	game.voiture.x = (game.voiture.w + game.ecart_obstacles)*6;

	game.rect_obstacle = malloc(sizeof(SDL_Rect)*game.nbVoiture);
	//init voiture dehors
	SDL_Rect rect_loin = {-100,300, 0,0};
	for (int i = 0; i < game.nbVoiture ; i++) {
		game.rect_obstacle[i] = rect_loin;
	}

	for (int i = 0; i < game.nbVoiture ; i++)
	{
		voitureAleatoire(&game, i, fenetre);
	}

	return game;
}

void voitureAleatoire(Game * game, int pos, SDL_Rect * fenetre)
{
    SDL_Rect obst;
    bool positionValide = false;


    
    while (!positionValide) {
        int yAlea = rand() % 100*game->nbVoiture;
        
		obst.w = 65;
		obst.h = 110;
        
		int xAlea = rand() % 12;
		
		obst.x = xAlea * (obst.w + game->ecart_obstacles);
		obst.y = -(yAlea+100) - game->ecart_obstacles;
		

		positionValide = true;
        
        for (int i = 0; i < game->nbVoiture; i++) {
			if (i != pos && SDL_HasIntersection(&game->rect_obstacle[i], &obst)) {
				positionValide = false;
				break;
			}
		}
    }
	
	obst.y += game->ecart_obstacles;

    game->rect_obstacle[pos] = obst;
}

void pieceAleatoire(Game * game, SDL_Rect * fenetre)
{
    SDL_Rect piece;
    bool positionValide = false;

    while (!positionValide) {
        int yAlea = rand() % 100;
        
		piece.w = 65;
		piece.h = 65;
        
		int xAlea = rand() % 12;
		
		piece.x = xAlea * (piece.w + game->ecart_obstacles);
		piece.y = -(yAlea+100) - game->ecart_obstacles;
		
		positionValide = true;
        
		if (SDL_HasIntersection(&game->rect_piece, &piece)) {
			positionValide = false;
			break;
			
		}
    }
	
	piece.y += game->ecart_obstacles;
    game->rect_piece = piece;
}


//Appelé une fois par frame
void game_update(Game* game,SDL_Rect* rect_fenetre,Uint32 deltatime){
	if (game->vie>0) {
		game->distance_parcouru += deltatime;
		game->vitesse = deplacer_obstacle(game,rect_fenetre,deltatime, game->distance_parcouru, game->nbVoiture);
	}	

	if (game->vie<=0) {
		game->delai_invulnerabilite = -1;
		return;
	}

	if (game->deplacement_voiture != 0) {
		game->temps_deplacement+=deltatime;
		deplaceVoiture(&game->voiture, game->ecart_obstacles, rect_fenetre, game->deplacement_voiture, deltatime*6.67);
		if (game->temps_deplacement > 150) {
			game->voiture.x = (game->voiture.w + game->ecart_obstacles) * ((game->voiture.x+game->voiture.w/2) / (game->voiture.w + game->ecart_obstacles));
			game->temps_deplacement = 0;
			game->deplacement_voiture = 0;
		}
	}
	deplacer_piece(game,rect_fenetre,deltatime,game->distance_parcouru);

	//Gerer collision ici
	if (game->delai_invulnerabilite<0) {
		if (test_collision(&game->voiture, game->rect_obstacle, game->nbVoiture)) {
			game->vie--;
			game->delai_invulnerabilite = game->delai_invulnerabilite_max;
		}
	}
	else {
		game->delai_invulnerabilite -= deltatime;
	}
}


void game_handle_event(Game* game, SDL_Event* event, SDL_Rect* rect_fenetre) {

	switch (event->type)
	{
		case SDL_KEYDOWN: {
			switch(event->key.keysym.sym)
			{
				case SDLK_LEFT:
				case SDLK_q : {
					game->deplacement_voiture = -1;
					break;
				}

				case SDLK_RIGHT:
				case SDLK_d : {
					game->deplacement_voiture = 1;
					break;
				}
			}
			break;
		}
		case SDL_KEYUP: {
			break;
		}
	default:
		break;
	}


}

void game_afficher(const Game* game, SDL_Renderer* renderer, SDL_Rect* rect_fenetre) {
	afficherRoute(renderer, game->textureHandler.textures[TEXTURE_Route], rect_fenetre, game->distance_parcouru);
	afficher_obstacle(renderer,game->rect_obstacle, (SDL_Texture**)game->textureHandler.textures,game->nbVoiture);
	afficher_piece(renderer,game->rect_piece, game->textureHandler.textures[8]);
	afficherVoiture(renderer,&game->voiture,game->textureHandler.textures[TEXTURE_voiture_course],game->deplacement_voiture*15, game->delai_invulnerabilite);
	if (game->vie <= 0) {
		afficherFin(renderer, rect_fenetre, game->distance_parcouru/175, game->vitesse/4, game->font);
		return;
	}
	afficher_texte(renderer, game->distance_parcouru, rect_fenetre, game->vitesse);
	afficherVie(renderer, game->textureHandler.textures[TEXTURE_Coeur_rouge], game->textureHandler.textures[TEXTURE_Coeur_gris], game->vie, game->vie_max, rect_fenetre);

	afficherEffetDegats(renderer, game->delai_invulnerabilite, game->delai_invulnerabilite_max, rect_fenetre);
}


void afficher_obstacle(SDL_Renderer* renderer, const SDL_Rect rect_obstacle[], SDL_Texture *textureObst[], int nombreVoiture)
{
	SDL_SetRenderDrawColor(renderer, 255,0,0,255);
	for (int i = 0; i < nombreVoiture; i++)
	{
		
		SDL_RenderCopyEx(renderer,textureObst[i%4],NULL,&rect_obstacle[i],0,NULL,SDL_FLIP_VERTICAL);
	}
	
}

void afficher_piece(SDL_Renderer* renderer, const SDL_Rect rect_piece, SDL_Texture *texturePiece)
{
	SDL_SetRenderDrawColor(renderer, 255,0,0,255);
	SDL_RenderCopyEx(renderer,texturePiece,NULL,&rect_piece,0,NULL,SDL_FLIP_VERTICAL);
}

void afficher_texte(SDL_Renderer* renderer,int dist,SDL_Rect* rect_fenetre, int vitesse){
	SDL_Rect fond_blanc = {rect_fenetre->w-92, 5, 90, 25};
	roundedBoxRGBA(renderer, fond_blanc.x, fond_blanc.y, fond_blanc.x + fond_blanc.w, fond_blanc.y + fond_blanc.h, 2, 255, 255, 255, 80);

	vitesse = vitesse / 4; //On divise par 4 pour "convertir" en km/h
	char vitesse_char[25];
	sprintf(vitesse_char, "%d KM/H", vitesse);

	char score[25];
	sprintf(score, "%d POINTS", dist/175);

	stringRGBA(renderer, rect_fenetre->w-90,10, vitesse_char, 0, 0, 20, 255);  //affichage texte
	stringRGBA(renderer, rect_fenetre->w-90,20, score, 0, 0, 20, 255);  //affichage texte
}

void liberer_game(Game* game) {
	if (game->font != NULL) {
		TTF_CloseFont(game->font);
		TTF_Quit();
	}
	free(game->rect_obstacle);
	freeTextureHandler(&game->textureHandler);
}

void deplaceVoiture(SDL_Rect* voiture, int ecart, SDL_Rect* fenetre, int direction_deplacement, Uint32 deltatime) {
    int limiteGauche = 0;  //lim gauche de la fenêtre
    int limiteDroite = 12*(voiture->w+ecart);  //lim gauche de la fenêtre
	
    voiture->x += (voiture->w+ecart) * direction_deplacement * (float)deltatime / 1000.0;
	if (voiture->x < limiteGauche) {
        voiture->x = limiteGauche;  //pixel vers la gauche
    }
	if (voiture->x+voiture->w > limiteDroite) {
		voiture->x = limiteDroite - voiture->w - ecart;
	}
}

int deplacer_obstacle(Game* game,SDL_Rect* rect_fenetre, Uint32 deltatime, int distance_parcouru , int nbVoiture){
	int vitesse = 100;
	
	vitesse += pow((distance_parcouru/150),1.10);
	if(vitesse > 1300)
	{
		vitesse = 1300;
		
	}
	//printf("vitesse %d \n",vitesse);
	for (int i = 0; i < nbVoiture; i++)
	{
		game->rect_obstacle[i].y+=vitesse*deltatime/1000.0;

		if(game->rect_obstacle[i].y > rect_fenetre->h)
		{
			voitureAleatoire(game,i,rect_fenetre);
		}
	}
	return vitesse;
		
}

void deplacer_piece(Game* game,SDL_Rect* rect_fenetre, Uint32 deltatime, int distance_parcouru){
	//printf("vitesse %d \n",vitesse);
	
	game->rect_piece.y+= game->vitesse*deltatime/1000.0;
	if(game->rect_piece.y > rect_fenetre->h)
	{
		pieceAleatoire(game,rect_fenetre);
	}

		
}

void afficherVoiture(SDL_Renderer* renderer, const SDL_Rect* voiture, SDL_Texture* textureVoiture, int inclinaison, int delai_invulnerabilite)
{
	if (delai_invulnerabilite<0 || delai_invulnerabilite%10 > 5) {
		SDL_RenderCopyEx(renderer,textureVoiture,NULL,voiture,inclinaison,NULL,SDL_FLIP_NONE);
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

void afficherEffetDegats(SDL_Renderer* renderer, const int delai_invulnerabilite, const int delai_invulnerabilite_max, const SDL_Rect* rect_fenetre) {
	if (delai_invulnerabilite > 0.8*delai_invulnerabilite_max) {
		SDL_SetRenderDrawColor(renderer, 200, 100, 100, 55);
		SDL_Rect rect = *rect_fenetre;
		rect.x = 0;
		rect.y = 0;
		SDL_RenderFillRect(renderer, &rect);
	}
}

bool test_collision(const SDL_Rect* voiture, const SDL_Rect rect_obstacle[], const int nbVoiture) {
	for (int i=0; i<nbVoiture; ++i) { //SDL_HasIntersectionF
		if (SDL_HasIntersection(voiture, &rect_obstacle[i])) {
			return true;
		}	
	}
	return false;
}

void afficherFin(SDL_Renderer* renderer, SDL_Rect* rect_fenetre, int score, int vitesse, TTF_Font* font) {
	roundedBoxRGBA(renderer, rect_fenetre->w*0.1, rect_fenetre->h*0.1, rect_fenetre->w*0.9, rect_fenetre->h*0.9, 10, 255, 255, 255, 80);
	char valStr[180];
	sprintf(valStr,"Score %d Vitesse max %d",score, vitesse);

	SDL_Color color = {10, 10, 10, 255};  
	SDL_Surface * surfaceTexte = TTF_RenderText_Solid(font, valStr, color);
	SDL_Texture * textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);


	int largeurTexte = surfaceTexte->w;
	int hauteurTexte = surfaceTexte->h;

	SDL_Rect rectDest;
	rectDest.x = rect_fenetre->w/2 - largeurTexte/2;
	rectDest.y = rect_fenetre->h/2 - hauteurTexte/2;
	rectDest.w = largeurTexte;
	rectDest.h = hauteurTexte;


	SDL_RenderCopy(renderer, textureTexte, NULL, &rectDest);   
	SDL_FreeSurface(surfaceTexte);  
	SDL_DestroyTexture(textureTexte); 
}


TTF_Font* charger_font() {
	if (TTF_Init() != 0)
	{
    	fprintf(stderr, "Erreur d'initialisation TTF : %s\n", TTF_GetError()); 
	}
	
	TTF_Font* font = NULL;       
	font = TTF_OpenFont("./font/8bit.ttf", 32);    
	if (font == NULL) fprintf(stderr, "Erreur d'initialisation TTF : %s\n", TTF_GetError()); 

	return font;	
}
