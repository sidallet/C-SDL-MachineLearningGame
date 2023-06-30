#include "fourmis.h"
#include <stddef.h>
#include <stdint.h>

double set_zero([[maybe_unused]]double v) {
	return 0;
}

double div_2(double v) {
	return v/2.0;
}

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

Chemin parcours_fourmis(const MatriceDouble proba_arc, const size_t nombre_noeuds)
{ //proba_arc(source)(dest)
	int i;
	Chemin chemin;
	chemin.val = (int*)malloc((nombre_noeuds + 1) * sizeof(int));
	int alea = 0;
	int list[nombre_noeuds+1];
	list[0] = rand() % nombre_noeuds; //rand
	double probaTotale = 0;

	for (int compt=1; compt<nombre_noeuds; ++compt)
	{
		for (i=0;i<nombre_noeuds;i++)
		{
			probaTotale += proba_arc[list[compt]][i];
			alea = probaTotale*((double) rand()) / RAND_MAX;
			
		}
		for (i=0;i<nombre_noeuds;i++)
		{
			if (alea < proba_arc[list[compt]][i] && exist(list,i,compt)==0)
			{
				list[compt]=i;
			}
			else {
				alea -=proba_arc[list[compt]][i];
			}
		}
	}

	list[nombre_noeuds] = list[0];
	
	printf("liste : ");
	for (i = 0; i < nombre_noeuds+1; i++)
	{
		printf("%d ", list[i]);
	}
	printf("\n");
	
	chemin.nombre_elems = nombre_noeuds;
	for (i = 0; i < nombre_noeuds + 1; i++) {
    chemin.val[i] = list[i];
	}
	return chemin;
}


int calcul_longueur_fourmis(const Matrice distances_graphe_complet, const size_t nombre_noeuds, const size_t nombre_iteration) {
	MatriceDouble proba_arcs = initMatriceDouble(nombre_noeuds);
	MatriceDouble pheromones_presentes = initMatriceDouble(nombre_noeuds);
	applyAllMatriceDouble(pheromones_presentes, nombre_noeuds, set_zero);
	MatriceDouble pheromones_ajoutees = initMatriceDouble(nombre_noeuds);
	
	int distance_min = UINT32_MAX;


	for (size_t i=0; i<nombre_iteration; ++i) {
		calculerProbaArcs(distances_graphe_complet, pheromones_presentes, &proba_arcs, nombre_noeuds);
		applyAllMatriceDouble(pheromones_ajoutees, nombre_noeuds, set_zero);
		
		//Parcours des fourmis
		for (size_t f=0; f<100; ++f) {
			Chemin chemin = parcours_fourmis(proba_arcs, nombre_noeuds);
		
			int distance = calculDistanceGrapheComplet(distances_graphe_complet, &chemin);
			//Dépot phéromone
			for (size_t k=1; k<chemin.nombre_elems; ++k) {
				pheromones_ajoutees[chemin.val[k-1]][chemin.val[k]] += 100.0/distance;
			}
		}	


		//Évaporation phéromones
		applyAllMatriceDouble(pheromones_presentes, nombre_noeuds, div_2);

		// Ajout phéromones
		for (size_t i = 0; i < nombre_noeuds; i++) {
			for (size_t j = 0; j < nombre_noeuds; j++) {
				pheromones_presentes[i][j] += pheromones_ajoutees[i][j];	
			}
		}
		
	}	


	libereMatriceDouble(proba_arcs, nombre_noeuds);
	libereMatriceDouble(pheromones_ajoutees, nombre_noeuds);
	libereMatriceDouble(pheromones_presentes, nombre_noeuds);

	return distance_min;
}
