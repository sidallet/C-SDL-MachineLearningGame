#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef int** Matrice;

void genereMatriceArbre(Matrice matrice, int bas, int haut);

Matrice initMatrice(const size_t nombre_points);

void afficheMatrice(Matrice matrice, const size_t nombre_points);

void libereMatrice(Matrice matrice, const size_t nombre_points);

void genereGraphe(Matrice matrice, float p, const size_t nombre_points);
