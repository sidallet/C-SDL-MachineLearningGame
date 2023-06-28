#include "voyageur.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "matrice.h"


int distance_eucli(Point point1,Point point2)
{
    int dist;
    dist =sqrt((point1.x-point2.x)*(point1.x-point2.x)+(point1.y-point2.y)*(point1.y-point2.y));
    return dist;
}

int genererNombreAleatoire(int N) {
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    return rand() % (N); // Utilisation de la fonction rand() pour générer un nombre aléatoire entre 0 et N-1
}




// void floydWarshall(int graph[V][V]) {
//     int dist[V][V];
//     int i, j, k;

//     for (i = 0; i < V; i++) {
//         for (j = 0; j < V; j++) {
//             dist[i][j] = graph[i][j];
//         }
//     }

//     for (k = 0; k < V; k++) {
//         for (i = 0; i < V; i++) {
//             for (j = 0; j < V; j++) {
//                 if (dist[i][k] + dist[k][j] < dist[i][j]) {
//                     dist[i][j] = dist[i][k] + dist[k][j];
//                 }
//             }
//         }
//     }

//     printf("Matrice de distances minimales :\n");
//     for (i = 0; i < V; i++) {
//         for (j = 0; j < V; j++) {
//             if (dist[i][j] == __INT16_MAX__) {
//                 printf("INF\t");
//             } else {
//                 printf("%d\t", dist[i][j]);
//             }
//         }
//         printf("\n");
//     }
// }

bool verifParcours(const int indicesPointSelect[], const size_t nb_indicesPointSelect, const size_t nombre_noeuds)
{
	if (!(nb_indicesPointSelect > 0 && indicesPointSelect[0] == indicesPointSelect[nb_indicesPointSelect - 1]))
	{
		return false;
	}

	for (size_t i = 0; i < nombre_noeuds; i++)
	{
		bool pointPresent = false;
		for (size_t j = 0; j < nb_indicesPointSelect; j++)
		{
			if (indicesPointSelect[j] == i)
			{
				pointPresent = true;
			}
		}
		if (!pointPresent)
		{
			return false;
		}
	}
	return true;
}


int existe(int val,int Liste[])
{int i;
int present=0;
    for(i=0;i<6;i++)
    {
        if (val==Liste[i])
        {
            present=1;
        }
    } 
    return present;
}



int recherche_local_glouton(int N,Point points[])
{   
    int dist_graphe_total=0;
    int i,j;
    //int Liste[N];        
    int * Liste = (int*)malloc(sizeof(int)*N+1);                                                                 // à changer avec le alloc (c'est la liste des des points qu'on a ajouté au fur et à mesure)
    int a=genererNombreAleatoire(N); 
    printf("a est : %d\n",a);                                                     //sommet aléatoire
    Point voisin_proche;
    Point point_a=points[a];
    Point point_deb=points[a];
    i=0;
 
    while (i<N-1)
    {
        {  
            int dist_graphe_sommet=INT16_MAX;                                               //on commence a la distance à l'infini(initialisation)
            for(j=0;j<N;j++)
            {            
                printf("----------------------- points actuel %d  -------------------\n", point_a.val);
                                                                                 //on commence à la ligne du sommet a puis on enchaine sur la ligne du sommet le plus proche
                Point point_j= points[j];                                                   //on va tester pour tout les points de la ligne a de la matrice d'adjacence
                if(i==4)
                {
                    printf("dernier !ii!!i!i!i!i!i!i!i\n");
                    dist_graphe_sommet=distance_eucli(point_deb,points[i]);
                }
                else if (point_j.val!=point_a.val&& existe(point_j.val,Liste)==0 )                 //ce n'est pas le sommet précédent
                {                                                                    //il y a un voisin qui n'est pas le précédent (donc pas un point isolé)
                    if (distance_eucli(point_a,point_j)<dist_graphe_sommet  ){                //si la distance entre le poitn a et j est plus faible on actualise la distance
                        dist_graphe_sommet=distance_eucli(point_a,point_j);
                        voisin_proche=point_j;                                                  //on a le nouveau voisin le plus proche qui n'est pas le précédent
                    }
                }                    
            }
            if (i==N-1)
            {
                printf("ici le i=N-1:");
                Liste[i]=point_a.val;
            }
            printf("i:%d\n",i);
            printf("point a ajouté: %d\n",voisin_proche.val); 
            
            
            Liste[i]=point_a.val;                                                       //on ajoute le point précédent dans la liste pour ne pas revenir en arriere.
            i=i+1;
            point_a=voisin_proche;                                                        //on actualise le point par le nouveau voisin le plus proche
            dist_graphe_total+=dist_graphe_sommet;
            printf("la distance est: %d\n",dist_graphe_total);                                          //on actualise la nouvelle distance du graphe
        } 

    printf("\n LISTE : ");
    for (int i = 0; i < N; i++)
    {
        printf("%d ", Liste[i]);
    }
    printf("\n");
    }  
    
    return dist_graphe_total;
}

// int fourmi(Matrice matrice,int N,Point points[])
// {
//     int j;
//     int proba;
//     Point point_actuel=points[0];
//     for(j=0;j<N;j++)
//     {   Point point_j= points[j]
//         int compt=0;
//         if (matrice[point_a.val][point_j.val]==1)
//         {
//         compt=compt+1;
//         }
//     }
//     for(j=0;j<N;j++)
//     {

//     }



// }


