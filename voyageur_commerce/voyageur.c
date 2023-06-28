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
    int dist_graphe_total=0;
    int i,j;
    
    int * Liste[100];// à changer avec le alloc
    int a=genererNombreAleatoire(N);//sommet aléatoire
    Point voisin_proche;
    Point point_a=points[a];
   
    while (verifparcours()==FALSE)
        {   i=0;
            int voisin_autre_présent==0;
            int dist_graphe_sommet=INT16_MAX;//on commence a la distance à l'infini(initialisation)
            for(j=0;j<N;j++)
            {//on commence à la ligne du sommet a puis on enchaine sur la ligne du sommet le plus proche
                Point point_j= points[j];//on va tester pour tout les points de la matrice d'ajacence
                if (matrice[point_a.val][point_j.val]==1 && Liste[i]!=point_j.val)//si il y a un lien entre les deux sommets et que ce n'est pas le sommet précédent
                {   
                    voisin_autre_présent=1;//il y a un voisin qui n'est pas le précédent (donc pas un point isolé)
                }  
                if (distance_eucli(point_a,point_j)>dist_graphe_sommet)
                {
                dist_graphe_sommet=distance_eucli(point_a,point_j);
                voisin_proche=point_j;
                }
                     
            }
            Liste[i]=point_a.val;//on ajoute le point précédent dans la liste pour pas "revenir en arrière" (problème si point isolé ?)
            i=i+1;
            point_a=voisin_proche;
            dist_graphe_total+=dist_graphe_sommet;
            
            
        }   
    return dist_graphe_total;
}



