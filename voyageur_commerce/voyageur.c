#include "voyageur.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
    return rand() % (N + 1); // Utilisation de la fonction rand() pour générer un nombre aléatoire entre 0 et N
}


int calculDistanceGraphe(Point * points, const int indicesPointSelect[], const size_t nombre_noeuds)
{
	int distTotal = 0;
	for (size_t i = 0; i+1 < nombre_noeuds; i++)
	{
		distTotal += distance_eucli(points[indicesPointSelect[i]], points[indicesPointSelect[i+1]]);
	}
	return distTotal;
}

Matrice calculMatriceDistance(Matrice adjacence, Point points[], size_t nombre_noeuds) {
	Matrice distances = initMatrice(nombre_noeuds);
	for (size_t i = 0; i < nombre_noeuds; i++) {
		for (size_t j = 0; j < nombre_noeuds; j++) {
			if (adjacence[i][j]) {
				distances[i][j] = distance_eucli(points[i], points[j]);
			}
			else if (i==j) {
				distances[i][j] = 0;
			}
			else {
				distances[i][j] = INT16_MAX;
			}
		}
	}

	return distances;
}

Matrice floydWarshall(Matrice graph, size_t nombre_noeuds) {
	Matrice dist = initMatrice(nombre_noeuds);
	int i, j, k;
	for (i = 0; i < nombre_noeuds; i++) {
		for (j = 0; j < nombre_noeuds; j++) {
			dist[i][j] = graph[i][j];
		}
	}
	for (k = 0; k < nombre_noeuds; k++) {
		for (i = 0; i < nombre_noeuds; i++) {
			for (j = 0; j < nombre_noeuds; j++) {
				if (dist[i][k] + dist[k][j] < dist[i][j]) {
					dist[i][j] = dist[i][k] + dist[k][j];
				}
			}
		}
	}

	return dist;
}

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




int recherche_local_glouton(Matrice matrice,int N,Point points[])
{   
    int dist_graphe_total=0;
    int i,j;
    
    int Liste[100];// à changer avec le alloc (c'est la liste des des points qu'on a ajouté au fur et à mesure)
    int a=genererNombreAleatoire(N);//sommet aléatoire
    Point voisin_proche;
    Point point_a=points[a];
    i=0;
    while (verifParcours(Liste,i,N)==false)
        {   printf("i:%d\n",i);
            
            int voisin_autre_présent=0;
            int dist_graphe_sommet=INT16_MAX;//on commence a la distance à l'infini(initialisation)
            for(j=0;j<N;j++)
            {//on commence à la ligne du sommet a puis on enchaine sur la ligne du sommet le plus proche
                Point point_j= points[j];//on va tester pour tout les points de la ligne a de la matrice d'adjacence
                if (matrice[point_a.val][point_j.val]==1 && Liste[i]!=point_a.val)//si il y a un lien entre les deux sommets et que ce n'est pas le sommet précédent
                {   
                    voisin_autre_présent=1;     //il y a un voisin qui n'est pas le précédent (donc pas un point isolé)
                    if (distance_eucli(point_a,point_j)<dist_graphe_sommet){        //si la distance entre le poitn a et j est plus faible on actualise la distance
                    dist_graphe_sommet=distance_eucli(point_a,point_j);
                    voisin_proche=point_j;   //on a le nouveau voisin le plus proche qui n'est pas le précédent
                    }
                }  
                if (voisin_autre_présent==0)// si c'est un point isolé on "revient en arrière"
                {   
                    voisin_proche.val=Liste[i];                   
                }                    
            }
            Liste[i]=point_a.val;//on ajoute le point précédent dans la liste pour "revenir en arrière" ou non.
            i=i+1;
            point_a=voisin_proche;//on actualise le point par le nouveau voisin le plus proche
            dist_graphe_total+=dist_graphe_sommet;//on actualise la nouvelle distance du graphe
        }   
    return dist_graphe_total;
}


Chemin generer_solution_initiale(const int nombre_noeuds) {
	Chemin chemin;
	chemin.nombre_elems = nombre_noeuds+1;
	if (nombre_noeuds==0) {
		chemin.val = NULL;
		return chemin;
	}
	chemin.val = (int*)malloc(sizeof(int)*(chemin.nombre_elems));
	if (chemin.val==NULL) {
		fprintf(stderr, "Erreur allocation\n");
		chemin.nombre_elems = 0;
		return chemin;
	}

	for (size_t i=0; i<chemin.nombre_elems-1;++i) {
		chemin.val[i] = i;
	}
	chemin.val[chemin.nombre_elems-1] = chemin.val[0];

	return chemin;
}

int calculDistanceGrapheComplet(const Matrice matrice, const Chemin* chemin) {
	int longeur = 0;
	for (size_t i=1; i<chemin->nombre_elems-1; ++i) {
		longeur+=matrice[chemin->val[i]][chemin->val[i-1]];
	}
	return longeur;
}

void afficheChemin(const Chemin* chemin) {
	for (size_t i=0; i<chemin->nombre_elems; ++i) {
		printf("%d ", chemin->val[i]);
	}
	printf("\n");
}



Chemin alterChemin(const Chemin* chemin) {
	int i = rand()%(chemin->nombre_elems-1);
	int	j = 1 + rand()%(chemin->nombre_elems-2);
	if (j == i) {
		j = 1+(j+1)%(chemin->nombre_elems-2); 
	}


	Chemin nouveau_chemin = {
		.val = malloc(sizeof(int)*chemin->nombre_elems),
		.nombre_elems = chemin->nombre_elems,
	};
	
	memcpy(nouveau_chemin.val, chemin->val, sizeof(int)*chemin->nombre_elems);
	
	nouveau_chemin.val[i] = chemin->val[j];
	nouveau_chemin.val[j] = chemin->val[i];
	if (i==0) {
		nouveau_chemin.val[chemin->nombre_elems-1] = chemin->val[j];
	}
	return nouveau_chemin;
}

bool recuit_impl(Chemin* chemin, const int longueurChemin, const Matrice matrice, double temperature, int* nouvelle_longeur) {
	Chemin nouveauChemin = alterChemin(chemin);
	*nouvelle_longeur = calculDistanceGrapheComplet(matrice, &nouveauChemin);
	
	if (longueurChemin > *nouvelle_longeur) {
		free(chemin->val);
		chemin->val = nouveauChemin.val;
		return true;
	}
	else {
		float p = exp(-(float)(*nouvelle_longeur - longueurChemin)/temperature);
		if ((double)rand() /INT32_MAX < p) {
			free(chemin->val);
			chemin->val = nouveauChemin.val;
			return true;	
		}
		else {
			return false;
		}
	}
}

double init_temperature(Matrice matrice, int nombre_noeud) {
	Chemin chemin = generer_solution_initiale(nombre_noeud);
	int longeur_max = calculDistanceGrapheComplet(matrice, &chemin);
	for (size_t i=0; i<20; ++i) {
		for(size_t j=0; j<nombre_noeud; ++j) {	
			Chemin nouveau_chemin = alterChemin(&chemin);
			free(chemin.val);
			chemin.val = nouveau_chemin.val;
		}
		int longeur = calculDistanceGrapheComplet(matrice, &chemin);
		afficheChemin(&chemin);
		if (longeur > longeur_max) {
			longeur_max = longeur;
		}
	}

	return longeur_max;
}

int recuit(Matrice matrice, int N, int nombre_iterations) {
	double temperature = init_temperature(matrice, N);
	printf("température initiale : %f\n", temperature);
	double pente = temperature/nombre_iterations;
	Chemin chemin = generer_solution_initiale(N);
	afficheChemin(&chemin);
	int longeurChemin = calculDistanceGrapheComplet(matrice, &chemin);

	while (temperature>0.001) {
		int nouvelle_longeur;
		if (recuit_impl(&chemin, longeurChemin, matrice, temperature, &nouvelle_longeur)) {
			longeurChemin = nouvelle_longeur;
		}
			
		temperature -= pente;
	}

	afficheChemin(&chemin);
	printf("Longueur %d\n", longeurChemin);

	free(chemin.val);
	return longeurChemin;
}

