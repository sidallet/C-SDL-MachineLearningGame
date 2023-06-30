#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef double** MatriceDouble;

MatriceDouble initMatriceDouble(const size_t nombre_points);

void afficheMatriceDouble(MatriceDouble matrice, const size_t nombre_points);

void libereMatriceDouble(MatriceDouble matrice, const size_t nombre_points);

void applyAllMatriceDouble(MatriceDouble matrice, const size_t nombre_points, double (*func)(const double));
