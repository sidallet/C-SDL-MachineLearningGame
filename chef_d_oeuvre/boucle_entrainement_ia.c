#include "boucle_entrainement_ia.h"
#include "game.h"
#include "regle.h"
#include <SDL2/SDL_events.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void affichage(SDL_Renderer* renderer, const Game* game, SDL_Rect* rect_fenetre) {
	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);
	game_afficher(game,renderer, rect_fenetre);
	SDL_RenderPresent(renderer);
}

void ia_think(Game* game, const TabRegle* tabRegle,SDL_Rect * fenetre) {
	if (game->deplacement_voiture != 0) {
		return; // En déplacement: pas de decision
	}
	Observation observation = creerObservation(game,game->rect_obstacle,game->nbVoiture,fenetre);
	
	Decision decision = choisirRegle(&observation, tabRegle);

	game->deplacement_voiture = decision;
}

int boucle_ia(const bool affichage_actif, TabRegle tabRegle, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager) {
	
	Game game = new_game(renderer, rect_fenetre);
	
	Uint32 delta_time = 0;
	if (!affichage_actif) {
		delta_time = 16;
	}

	while (true) {
		if (affichage_actif) {
			SDL_Event event;
			while(SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					return 0;
				}	
			}
		}

		ia_think(&game, &tabRegle,rect_fenetre);
		game_update(&game, rect_fenetre, delta_time);
		
		if (game.vie == 0 || game.distance_parcouru >= 100000) {
			return game.distance_parcouru;
		}

		if (affichage_actif) {
			affichage(renderer, &game, rect_fenetre);
			delta_time = SDL_framerateDelay(fpsManager);
		}
	}

	liberer_game(&game);
}
Observation creerObservation(const Game* game,const SDL_Rect rect_obstacle[],const int nbVoiture,SDL_Rect * fenetre)
{
	Observation situation;
	int i;
	SDL_Rect rect_Loin[12];
	SDL_Rect rect_proche[12];//x y largeur hauteur
	bool bool_loin=false;
	bool bool_proche=false;
	for(i=0;i<12;i++)
	{
		rect_Loin[i].h=(game->voiture.w + game->ecart_obstacles)*i;
		rect_Loin[i].w=0;
		rect_Loin[i].x=game->voiture.w;
		rect_Loin[i].y=fenetre->h/2;


		rect_proche[i].h =(game->voiture.w + game->ecart_obstacles)*i;
		rect_proche[i].w=fenetre->h/2;
		rect_proche[i].x=game->voiture.w;
		rect_proche[i].y=fenetre->h/2;
	}

	for (i=-2;i<3;i++)//les 5 colonnes à étudier de -2 à 2
	{
		if (game->voiture.x+i*(game->voiture.w + game->ecart_obstacles)<0 || game->voiture.x+i*(game->voiture.w + game->ecart_obstacles)>fenetre->h)
		{
			situation.routes[i+2]=2;
		}
		else
		{
		 	bool_loin=test_collision(&rect_Loin[game->voiture.x+i*(game->voiture.w + game->ecart_obstacles)],game->rect_obstacle,nbVoiture);    
			bool_proche=test_collision(&rect_proche[game->voiture.x+i*(game->voiture.w + game->ecart_obstacles)],game->rect_obstacle,nbVoiture);
			{
				if (bool_proche==true)
				{
					situation.routes[i+2]=2;
				}
				else if(bool_proche==false && bool_proche==true) 
				{
					situation.routes[i+2]=1;
				}
				else if (bool_loin==false && bool_proche==false)
				{
					situation.routes[i+2]=0;
				}

			}	
	}
	}
	afficherObservation(stdout,situation);
	return situation;

}



Decision choisirRegle(const Observation* observation, const TabRegle* tabRegle) {
	// Selection
	size_t indice_regles_match[NB_REGLES];
	size_t nb_regles_match = 0;
	for (size_t i=0; i<NB_REGLES; ++i) {
		if (observation_match(*observation, tabRegle->regles[i].observ)) {
			indice_regles_match[nb_regles_match] = i;
			nb_regles_match++;
		}	
	}

	// Tirage
	int cumul_priorite = 0;
	printf("Obs \n");
	for (size_t i=0; i<nb_regles_match; ++i) {
		afficherObservation(stdout, tabRegle->regles[indice_regles_match[i]].observ);
		printf("\n");
		cumul_priorite += tabRegle->regles[indice_regles_match[i]].priorite;
	}

	float alea = ((float)rand()) / RAND_MAX;
	for (size_t i=0; i<nb_regles_match; ++i) {
		float probaI = ((float)tabRegle->regles[indice_regles_match[i]].priorite) / cumul_priorite;
		if (alea < probaI) {
			return tabRegle->regles[indice_regles_match[i]].decis;
		}
		alea -= probaI;
	}

	return RIEN;
}

bool observation_match(const Observation obs1, const Observation obs2) {
	for (size_t i=0; i<5; ++i) {
		if (obs1.routes[i] == JOKER || obs2.routes[i] == JOKER) {
			continue;
		}
		if (obs1.routes[i] != obs2.routes[i]) {
			return false;
		}
	}

	return true;
}
