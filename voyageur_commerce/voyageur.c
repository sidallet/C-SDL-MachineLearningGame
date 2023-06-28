#include "voyageur.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>


int distance_eucli(Point point1,Point point2)
{
    int dist;
    dist =sqrt((point1.x-point2.x)*(point1.x-point2.x)+(point1.y-point2.y)*(point1.y-point2.y));
    return dist;
}




int genererNombreAleatoire(int N) {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    return rand() % (N + 1); // Utilisation de la fonction rand() pour générer un nombre aléatoire entre 0 et N
}




void floydWarshall(int graph[V][V]) {
    int dist[V][V];
    int i, j, k;

    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    for (k = 0; k < V; k++) {
        for (i = 0; i < V; i++) {
            for (j = 0; j < V; j++) {
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printf("Matrice de distances minimales :\n");
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (dist[i][j] == __INT16_MAX__) {
                printf("INF\t");
            } else {
                printf("%d\t", dist[i][j]);
            }
        }
        printf("\n");
    }
}

int recherche_local(Matrice matrice,int N,Point points[])
{   
    int dist_graphe_sommet=INT16_MAX;//on commence a la distance 0 (initialisation)
    int dist_graphe_total=0;
    int i,j;
    int * Liste[]
    Point voisin_proche;
    int a=genererNombreAleatoire(N);//sommet aléatoire
    Point point_a=points[a];
   
    while (verifparcours()==FALSE)
        {   
            dist_graphe_sommet=0;//on remet à 0 car on revient sur un nouveau sommet
            for(j=0;j<N;j++)
            {//on commence à la ligne du sommet a
                Point point_j= points[j];
                if (matrice[point_a.val][point_j.val]==1)
                {
                    if (distance_eucli(point_a,point_j)>dist_graphe_sommet)
                    {
                    dist_graphe_sommet=distance_eucli(point_a,point_j);
                    voisin_proche=point_j;
                    }
                }      
            }
            point_a=voisin_proche;
            dist_graphe_total+=dist_graphe_sommet;
            in
            
        }   
    return dist_graphe_total;
}



