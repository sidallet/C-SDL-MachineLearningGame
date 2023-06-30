#include "matrice_double.h" 
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


MatriceDouble initMatrice(const size_t nombre_points)
{
    MatriceDouble matrice = (MatriceDouble)malloc(nombre_points * sizeof(double*));
	if (matrice==NULL) {
		fprintf(stderr, "Erreur malloc graphe\n");
		return NULL;
	}

    for (size_t i = 0; i < nombre_points; i++) {
        matrice[i] = (double*)calloc(nombre_points, sizeof(double));
		if (matrice[i]==NULL) {
			for (; i!=0; --i) {
				free(matrice[i-1]);
			}
			free(matrice);
			fprintf(stderr, "Erreur malloc graphe\n");
			return NULL;
		}
    }

	return matrice;
}

/**
 * Permet d'afficher une matrice
 * matrice : matrice à afficher
 * nombre_points : nombres de sommets
 * */
void afficheMatriceDouble(MatriceDouble matrice, const size_t nombre_points)
{
    for (size_t i = 0; i < nombre_points; i++) {
        for (size_t j = 0; j < nombre_points; j++) {
            printf("%f ", matrice[i][j]);
        }
        printf("\n");
    }
}

/**
 * Permet de liberer une matrice
 * matrice : matrice à liberer
 * nombre_points : nombres de sommets
 * */
void libereMatriceDouble(MatriceDouble matrice, const size_t nombre_points)
{
    for (size_t i = 0; i < nombre_points; i++) {
        free(matrice[i]);
    }
    free(matrice);
}
        
            
