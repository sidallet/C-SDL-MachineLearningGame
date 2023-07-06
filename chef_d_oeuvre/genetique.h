#ifndef GENETIQUE_H_INCLUDED
#define GENETIQUE_H_INCLUDED

#include "regle.h"
#include <SDL2/SDL_rect.h>

TabRegle genetique(int nombre_iterations, SDL_Rect * rect_fenetre, size_t nb_parties, TabRegle tabRegle);

#endif //GENETIQUE_H_INCLUDED
