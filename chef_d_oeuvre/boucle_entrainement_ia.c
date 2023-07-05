#include "boucle_entrainement_ia.h"
#include "game.h"
#include "regle.h"
#include <SDL2/SDL2_framerate.h>
#include <stdbool.h>
#include <stdio.h>


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
	ObservationPiece observationPiece = creerObservationPiece(game, fenetre);
	
	Decision decision = choisirRegle(&observation,&observationPiece ,tabRegle);
	game->deplacement_voiture = decision;
}

int boucle_ia(const bool affichage_actif, TabRegle tabRegle, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager) {
	
	Game game = new_game(affichage_actif, renderer, rect_fenetre);
		
	Uint32 delta_time = 0;
	if (!affichage_actif) {
		delta_time = 16;
	}
	else {
		SDL_initFramerate(fpsManager);
		SDL_setFramerate(fpsManager, 60);
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
		
		if (game.vie == 0 || game.distance_parcouru >= 1000000) {
			return calculerScore(&game);
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
	bool bool_loin=false;
	bool bool_moyen=false;
	bool bool_proche=false;
	bool bool_contact=false;
	const int colonnes_voiture=game->voiture.x/(game->voiture.w+game->ecart_obstacles);
	
	SDL_Rect rect_loin = { //Rect en haut de la fenetre
		.x=(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=0,
		.w=game->voiture.w,
		.h=fenetre->h/4,
	};

	SDL_Rect rect_moyen = { //Rect en dessous du rect_loin
		.x =(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=fenetre->h/4,
		.w=game->voiture.w,
		.h=fenetre->h/4
	};

	SDL_Rect rect_proche = { //Rect en dessous du milieu
		.x =(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=fenetre->h/2,
		.w=game->voiture.w,
		.h=fenetre->h/4
	};

	SDL_Rect rect_contact = { //Rect tout en bas
		.x =(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=(3*fenetre->h)/4,
		.w=game->voiture.w,
		.h=fenetre->h/4
	};

	

	for (int i=-2;i<3;i++)//les 5 colonnes à étudier de -2 à 2
	{
		if (colonnes_voiture+i<0 || colonnes_voiture+i>12)
		{
			situation.routes[i+2] = CONTACT;	
		}
		else
		{
		 	bool_loin=test_collision(&rect_loin,game->rect_obstacle,nbVoiture);   
			bool_moyen=test_collision(&rect_moyen,game->rect_obstacle,nbVoiture); 
			bool_proche=test_collision(&rect_proche,game->rect_obstacle,nbVoiture);	
			bool_contact=test_collision(&rect_contact,game->rect_obstacle,nbVoiture);	
			if (bool_contact==true) //La voiture est tres proche
			{
				situation.routes[i+2]=CONTACT;
			}
			else if(bool_proche==true) //La voiture est proche
			{
				situation.routes[i+2]=PROCHE;
			}
			else if(bool_moyen==true) //La voiture est assez loin
			{
				situation.routes[i+2]=MOYEN;
			}
			else if(bool_loin==true) //La voiture est très loin
			{
				situation.routes[i+2]=LOIN;
			}
			else // (bool_loin==false && bool_proche==false) Pas de voiture
			{
				situation.routes[i+2]=VIDE;
			}

		}
		rect_loin.x += game->voiture.w + game->ecart_obstacles;
		rect_proche.x += game->voiture.w + game->ecart_obstacles;
		rect_contact.x += game->voiture.w + game->ecart_obstacles;
		rect_moyen.x += game->voiture.w + game->ecart_obstacles;
	}
	return situation;

}

ObservationPiece creerObservationPiece(const Game* game, SDL_Rect * fenetre)
{
	ObservationPiece situationPiece;
	situationPiece.presence = VIDE;
	situationPiece.colonne = 0;
	bool bool_loin=false;
	bool bool_moyen=false;
	bool bool_proche=false;
	bool bool_contact=false;
	const int colonnes_voiture=game->voiture.x/(game->voiture.w+game->ecart_obstacles);
	
	SDL_Rect rect_loin = { //Rect en haut de la fenetre
		.x=(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=0,
		.w=game->voiture.w,
		.h=fenetre->h/4,
	};

	SDL_Rect rect_moyen = { //Rect en dessous du rect_loin
		.x =(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=fenetre->h/4,
		.w=game->voiture.w,
		.h=fenetre->h/4
	};

	SDL_Rect rect_proche = { //Rect en dessous du milieu
		.x =(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=fenetre->h/2,
		.w=game->voiture.w,
		.h=fenetre->h/4
	};

	SDL_Rect rect_contact = { //Rect tout en bas
		.x =(game->voiture.w + game->ecart_obstacles)*(colonnes_voiture-2),
		.y=(3*fenetre->h)/4,
		.w=game->voiture.w,
		.h=fenetre->h/4
	};

	for (int i=-2;i<3;i++)//les 5 colonnes à étudier de -2 à 2
	{
		if (colonnes_voiture+i<0 || colonnes_voiture+i>12)
		{
			continue;	
		}
		else
		{
		 	bool_loin=test_collisionPiece(&rect_loin,game->rect_piece);   
			bool_moyen=test_collisionPiece(&rect_loin,game->rect_piece);
			bool_proche=test_collisionPiece(&rect_loin,game->rect_piece);	
			bool_contact=test_collisionPiece(&rect_loin,game->rect_piece);	
			if (bool_contact==true) //La voiture est tres proche
			{
				situationPiece.presence = CONTACT;
				situationPiece.colonne = i;
			}
			else if(bool_proche==true) //La voiture est proche
			{
				situationPiece.presence = PROCHE;
				situationPiece.colonne = i;
			}
			else if(bool_moyen==true) //La voiture est assez loin
			{
				situationPiece.presence = MOYEN;
				situationPiece.colonne = i;
			}
			else if(bool_loin==true) //La voiture est très loin
			{
				situationPiece.presence = LOIN;
				situationPiece.colonne = i;
			}

		}
		rect_loin.x += game->voiture.w + game->ecart_obstacles;
		rect_proche.x += game->voiture.w + game->ecart_obstacles;
		rect_contact.x += game->voiture.w + game->ecart_obstacles;
		rect_moyen.x += game->voiture.w + game->ecart_obstacles;
	}
	return situationPiece;

}


Decision choisirRegle(const Observation* observation, const ObservationPiece* observationPiece,const TabRegle* tabRegle) {
	// Selection
	size_t indice_regles_match[NB_REGLES];
	size_t nb_regles_match = 0;
	for (size_t i=0; i<NB_REGLES; ++i) {
		if (observation_match(*observation, tabRegle->regles[i].observ) && observationPiece_match(*observationPiece, tabRegle->regles[i].obsPiece) ) {
			indice_regles_match[nb_regles_match] = i;
			nb_regles_match++;
		}	
	}

	// Tirage
	int cumul_priorite = 0;
	for (size_t i=0; i<nb_regles_match; ++i) {
		cumul_priorite += pow(tabRegle->regles[indice_regles_match[i]].priorite, 2);
	}

	float alea = ((float)rand()) / RAND_MAX;
	for (size_t i=0; i<nb_regles_match; ++i) {
		float probaI = pow(((float)tabRegle->regles[indice_regles_match[i]].priorite), 2) / cumul_priorite;
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

bool observationPiece_match(const ObservationPiece obs1, const ObservationPiece obs2) {
	if(obs1.presence == JOKER || obs2.presence == JOKER)
	{
		return obs1.colonne==obs2.colonne;
	}
	if(obs1.presence != obs2.presence)
	{
		return false;
	}

	if(obs1.presence == VIDE)
	{
		return false;
	}
	return obs1.colonne == obs2.colonne;

}

