#include "matrice.h" 
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void genereMatriceArbre(Matrice matrice, int bas, int haut) {
    if (bas < haut) {
        int k = (rand() % (haut - bas)) + bas + 1;
        matrice[bas][bas+1] = 1;
        matrice[bas+1][bas] = 1;
        if (k+1 <= haut) {
            matrice[bas][k+1] = 1;
            matrice[k+1][bas] = 1;
        }
        genereMatriceArbre(matrice, bas+1, k);
        genereMatriceArbre(matrice, k+1, haut); 
    }
}

Matrice initMatrice(const size_t nombre_points)
{
    Matrice matrice = (Matrice)malloc(nombre_points * sizeof(int*));
	if (matrice) {
		fprintf(stderr, "Erreur malloc graphe\n");
		return NULL;
	}

    for (size_t i = 0; i < nombre_points; i++) {
        matrice[i] = (int*)calloc(nombre_points, sizeof(int));
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

void afficheMatrice(Matrice matrice, const size_t nombre_points)
{
    for (size_t i = 0; i < nombre_points; i++) {
        for (size_t j = 0; j < nombre_points; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}

void libereMatrice(Matrice matrice, const size_t nombre_points)
{
    for (size_t i = 0; i < nombre_points; i++) {
        free(matrice[i]);
    }
    free(matrice);
}


void genereGraphe(Matrice matrice, float p, const size_t nombre_points)
{
    float randomVal;

    for (size_t i = 0; i < nombre_points; i++) {
        for (size_t j = 0; j < nombre_points; j++) {
            randomVal = (float)rand() / RAND_MAX;
            
            if (randomVal < p )
            {
                matrice[i][j] = 1;
                matrice[j][i] = 1;
            }
        }
    }
}
    

    
        
            
