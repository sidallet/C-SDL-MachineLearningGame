#include "matrice.h" 
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Generer une matrice d'adjacence sous forme d'arbre
 * matrice : matrice d'adjacence
 *
**/
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

/**
 * Permet d'initialiser une matrice correspond à un graphe
 * nombre_points : nombre de sommet du graphe
 **/
Matrice initMatrice(const size_t nombre_points)
{
    Matrice matrice = (Matrice)malloc(nombre_points * sizeof(int*));
	if (matrice==NULL) {
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

/**
 * Permet d'afficher une matrice
 * matrice : matrice à afficher
 * nombre_points : nombres de sommets
 * */
void afficheMatrice(Matrice matrice, const size_t nombre_points)
{
    for (size_t i = 0; i < nombre_points; i++) {
        for (size_t j = 0; j < nombre_points; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}

/**
 * Permet de liberer une matrice
 * matrice : matrice à liberer
 * nombre_points : nombres de sommets
 * */
void libereMatrice(Matrice matrice, const size_t nombre_points)
{
    for (size_t i = 0; i < nombre_points; i++) {
        free(matrice[i]);
    }
    free(matrice);
}

/**
 * Permet de generer un graphe en rajoutant des arretes
 * matrice : matrice à modifier
 * p : parametre pour avoir plus ou moins d'aretes
 * nombre_points : nombres de sommets
 * */
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
    

    
        
            
