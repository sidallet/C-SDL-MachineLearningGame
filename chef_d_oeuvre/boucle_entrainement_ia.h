#ifndef BOUCLE_ENTRAINEMENT_IA_H
#define BOUCLE_ENTRAINEMENT_IA_H

 
#include "game.h"
#include "regle.h"
#include <SDL2/SDL2_framerate.h>

void ia_think(Game* game, const TabRegle* tabRegle);

int boucle_ia(const bool affichage_actif, TabRegle tabRegle, SDL_Rect* rect_fenetre, SDL_Renderer* renderer, FPSmanager* fpsManager);


Observation creerObservation(const Game* game);
Decision choisirRegle(const Observation* observation, const TabRegle* tabRegle);

bool observation_match(const Observation obs1, const Observation obs2);

#endif // BOUCLE_ENTRAINEMENT_IA_H
