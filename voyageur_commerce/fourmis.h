#ifndef FOURMIS_INCLUDED
#define FOURMIS_INCLUDED

#include "matrice.h"
#include "matrice_double.h"
#include "voyageur.h"


/***
 * @param[out] pheromones
 **/
void calculerProbaArcs(const Matrice distances_graphe_complet, const MatriceDouble pheromones, MatriceDouble* proba_arcs, const size_t nombre_noeuds);

int calcul_longueur_fourmis(const Matrice distances_graphe_complet, const size_t nombre_noeuds, const size_t nombre_iteration);

#endif //FOURMIS_INCLUDED
