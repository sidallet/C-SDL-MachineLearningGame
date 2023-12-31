#include "boucle_entrainement_ia.h"
#include "game.h"
#include "regle.h"
#include <SDL2/SDL2_framerate.h>
#include <stdbool.h>
#include <stdio.h>
#include <threads.h>

void afficherObservationHitbox(const Game * game, SDL_Renderer * renderer,SDL_Rect * rect_fenetre,Observation obs,  int colVoiture)
{
	SDL_Rect rect_affich;

	rect_affich.w = game->voiture.w;
	rect_affich.h = rect_fenetre->h/4;

	for (int i = -2; i < 3; i++)
	{
		SDL_SetRenderDrawColor(renderer, 0 + ((rect_fenetre->h/4)/3 * (obs.routes[i+2] - 1)) , 150 - (rect_fenetre->h/4)/3, 0, 175);
		rect_affich.x = (game->voiture.w + game->ecart_obstacles) * (i+colVoiture);
		rect_affich.y = (rect_fenetre->h/4) * (obs.routes[i+2] - 1);
		SDL_RenderFillRect(renderer, &rect_affich);
	}
}

void affichage(SDL_Renderer* renderer, const Game* game, SDL_Rect* rect_fenetre, Observation obs, int colVoiture, bool vision) {
	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);
	game_afficher(game,renderer, rect_fenetre);
	if(vision) afficherObservationHitbox(game,renderer,rect_fenetre,obs,colVoiture);
	SDL_RenderPresent(renderer);
}

Observation ia_think(Game* game, const TabRegle* tabRegle,SDL_Rect * fenetre) {
	if (game->deplacement_voiture != 0) {
		return; // En déplacement: pas de decision
	}
	Observation observation = creerObservation(game,fenetre);
	ObservationPiece observationPiece = creerObservationPiece(game, fenetre);
	game->deplacement_voiture = choisirRegle(&observation,&observationPiece ,tabRegle);
	return observation;
}

int boucle_ia(const bool affichage_actif, TabRegle tabRegle, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager) {
	Observation obs;
	bool vision = false;
	Game game = new_game(affichage_actif, renderer, rect_fenetre);
		
	Uint32 delta_time = 0;
	if (!affichage_actif) {
		delta_time = 16;
	}
	else {
		SDL_initFramerate(fpsManager);
		SDL_setFramerate(fpsManager, 60);
	}
	bool running = true;
	while (running) {
		if (affichage_actif) {
			SDL_Event event;
			while(SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					running = false;
				}	
				if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F3) {
					vision = !vision;
				}
			}
		}
		const int colVoiture = game.voiture.x/(game.voiture.w+game.ecart_obstacles);
		obs = ia_think(&game, &tabRegle,rect_fenetre);
		game_update(&game, rect_fenetre, delta_time);
		
		if (game.vie == 0 || game.distance_parcouru >= 1000000) {
			running = false;
		}

		if (affichage_actif) {
			affichage(renderer, &game, rect_fenetre, obs,colVoiture, vision);
			delta_time = SDL_framerateDelay(fpsManager);
		}
	}

	const int score = calculerScore(&game);
	liberer_game(&game);
	return score;
}

Observation creerObservation(const Game* game, SDL_Rect * fenetre)
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
		 	bool_loin=test_collision(&rect_loin,game->rect_obstacle, game->nbVoiture);   
			bool_moyen=test_collision(&rect_moyen,game->rect_obstacle, game->nbVoiture); 
			bool_proche=test_collision(&rect_proche,game->rect_obstacle, game->nbVoiture);	
			bool_contact=test_collision(&rect_contact,game->rect_obstacle, game->nbVoiture);	
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
			bool_moyen=test_collisionPiece(&rect_moyen,game->rect_piece);
			bool_proche=test_collisionPiece(&rect_proche,game->rect_piece);	
			bool_contact=test_collisionPiece(&rect_contact,game->rect_piece);	
			if (bool_contact==true) //La voiture est tres proche
			{
				situationPiece.presence = CONTACT;
				situationPiece.colonne = i;
				break;
			}
			else if(bool_proche==true) //La voiture est proche
			{
				situationPiece.presence = PROCHE;
				situationPiece.colonne = i;
				break;
			}
			else if(bool_moyen==true) //La voiture est assez loin
			{
				situationPiece.presence = MOYEN;
				situationPiece.colonne = i;
				break;
			}
			else if(bool_loin==true) //La voiture est très loin
			{
				situationPiece.presence = LOIN;
				situationPiece.colonne = i;
				break;
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
		cumul_priorite += pow(tabRegle->regles[indice_regles_match[i]].priorite, 4);
	}

	float alea = ((float)rand()) / RAND_MAX;
	for (size_t i=0; i<nb_regles_match; ++i) {
		float probaI = pow(((float)tabRegle->regles[indice_regles_match[i]].priorite), 4) / cumul_priorite;
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
	if(obs1.presence == JOKER 
			|| obs2.presence == JOKER)
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

struct ParamsBoucleIA {
	TabRegle tabRegle;
	SDL_Rect rect_fenetre;
	size_t nb_parties;
};

int wrap_boucle_ia(void* params) {
	struct ParamsBoucleIA* p = (struct ParamsBoucleIA*) params;
	int sum = 0;
	for (size_t i=0; i<p->nb_parties; ++i) {
		sum += boucle_ia(false, p->tabRegle, &p->rect_fenetre, NULL, NULL);
	}
	return sum;
}

int multi_boucle_ia(TabRegle tabRegle, SDL_Rect* rect_fenetre, size_t nb_parties) {
	int sum = 0;
	thrd_t threads_handles[NUM_THREADS];
	int retours[NUM_THREADS];


	for (int i=0; i<NUM_THREADS; ++i) {
		threads_handles[i] = 0;
		retours[i] = 0;
	}
	struct ParamsBoucleIA params = {
		.tabRegle = tabRegle,
		.rect_fenetre = *rect_fenetre,
		.nb_parties = nb_parties/NUM_THREADS
	};

    for (int i = 0; i < NUM_THREADS; i++) {
        if (thrd_create(&threads_handles[i], wrap_boucle_ia, (void *)&params) != thrd_success) {
			fprintf(stderr, "Erreur création thread\n");
		}
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        thrd_join(threads_handles[i], &retours[i]);
		sum += retours[i];
    }

	return sum / nb_parties;
}


