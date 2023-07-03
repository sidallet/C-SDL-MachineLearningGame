#ifndef BOUCLE_ENTRAINEMENT_IA_H
#define BOUCLE_ENTRAINEMENT_IA_H

 
#include "game.h"
#include "regle.h"
#include <SDL2/SDL2_framerate.h>

void ia_think(Game* game, const TabRegle* tabRegle,SDL_Rect * fenetre);

int boucle_ia(const bool affichage_actif, TabRegle tabRegle, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager);


Observation creerObservation(const Game* game,const SDL_Rect rect_obstacle[],const int nbVoiture,SDL_Rect * fenetre);
Decision choisirRegle(const Observation* observation, const TabRegle* tabRegle);


#endif // BOUCLE_ENTRAINEMENT_IA_H
