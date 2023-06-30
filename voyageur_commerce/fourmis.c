#include "fourmis.h"
#include <stddef.h>
#include <stdint.h>

void calculerProbaArcs(const Matrice distances_graphe_complet, const MatriceDouble pheromones, MatriceDouble* proba_arcs, const size_t nombre_noeuds) {
	for (size_t i=0; i<nombre_noeuds; ++i) {
		double sum = 0;
		for (size_t j=0; j<nombre_noeuds; ++j) {
			*(proba_arcs[i][j]) = (100+pheromones[i][j])/distances_graphe_complet[i][j];
			sum += *(proba_arcs[i][j]);
		}
		for (size_t j=0; j<nombre_noeuds; ++j) {	
			*(proba_arcs[i][j]) /= sum;
		}
	}
}

Chemin parcours_fourmis(const MatriceDouble proba_arc, const size_t nombre_noeuds) {
	// proba_arc[source][dest]

} 

int longueur_fourmis(const Matrice distances_graphe_complet, const size_t nombre_noeuds, const size_t nombre_iteration) {
	Matrice proba_arcs = initMatrice(nombre_noeuds);
	MatriceDouble pheromones_presentes = initMatriceDouble(nombre_noeuds); 
	MatriceDouble pheromones_ajoutees = initMatriceDouble(nombre_noeuds);
	int distance_min = UINT32_MAX;

	calculerProbaArcs(distances_graphe_complet, pheromones_presentes, &proba_arcs, nombre_noeuds);

	


	libereMatrice(proba_arcs, nombre_noeuds);
	libereMatrice(pheromones_ajoutees, nombre_noeuds);
	libereMatrice(pheromones_presentes, nombre_noeuds);

	return distance_min;
}
