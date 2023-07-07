#ifndef BOUCLE_ENTRAINEMENT_IA_H
#define BOUCLE_ENTRAINEMENT_IA_H

 
#include "game.h"
#include "regle.h"
#include <SDL2/SDL2_framerate.h>

#define NUM_THREADS 8

Observation ia_think(Game* game, const TabRegle* tabRegle,SDL_Rect * fenetre, SDL_Rect* v, int* deplacement_v, int vnum);

int boucle_ia(const bool affichage_actif, TabRegle tabRegle, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager);

int multi_boucle_ia(TabRegle tabRegle, SDL_Rect* rect_fenetre, size_t nb_parties);

Observation creerObservation(const Game* game, SDL_Rect* v, SDL_Rect * fenetre);
ObservationPiece creerObservationPiece(const Game* game, SDL_Rect* v, SDL_Rect * fenetre);
Decision choisirRegle(const Observation* observation, const ObservationPiece* observationPiece, int colone_relative,const TabRegle* tabRegle);

bool observation_match(const Observation obs1, const Observation obs2);
bool observationPiece_match(const ObservationPiece obs1, const ObservationPiece obs2);

void affichageJoueur(SDL_Renderer* renderer, const Game* game, SDL_Rect* rect_fenetre);

#endif // BOUCLE_ENTRAINEMENT_IA_H
