#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

void genereMatriceArbre(int** matrice, int bas, int haut);

void initMatrice(int ** matrice);

void afficheMatrice(int ** matrice);

void libereMatrice(int ** matrice);

void genereGraphe(int ** matrice, float p);