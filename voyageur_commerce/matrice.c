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
        matrice[i] = (int*)calloc(N, sizeof(int));
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


void genereGraphe(int ** matrice, float p)
{
    float randomVal;
    int i,j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            randomVal = (float)rand() / RAND_MAX;
            
            if (randomVal < p )
            {
                matrice[i][j] = 1;
                matrice[j][i] = 1;
            }
        }
    }
}
    

    
        
            
