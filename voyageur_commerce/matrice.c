#include "matrice.h" 


void genereMatriceArbre(int** matrice, int bas, int haut) {
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

void initMatrice(int ** matrice)
{
    int i;

    for (i = 0; i < N; i++) {
        matrice[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            matrice[i][j] = 0;
        }
    }
}

void afficheMatrice(int ** matrice)
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
}

void libereMatrice(int ** matrice)
{
    for (int i = 0; i < N; i++) {
        free(matrice[i]);
    }
    free(matrice);
}