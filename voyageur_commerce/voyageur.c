#include "voyageur.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "matrice.h"

int distance_eucli(Point point1,Point point2)
{
    int dist;
    dist =sqrt((point1.x-point2.x)*(point1.x-point2.x)+(point1.y-point2.y)*(point1.y-point2.y));
    return dist;
}

SDL_Rect create_rect_cadre(const size_t num_cadre, const size_t nb_colones, const size_t nb_lignes, const Uint32 w_zone, const Uint32 h_zone) {
	size_t ligne = num_cadre/nb_colones;
	size_t colone = num_cadre%nb_colones;
	SDL_Rect rect;
	rect.w = w_zone/nb_lignes; 
	rect.h = h_zone/nb_colones; 
	rect.x = ligne*rect.w;
	rect.y = colone*rect.h;
	return rect;
}

void generer_points(Point points[], const size_t nombre_points, const Uint32 w_zone, const Uint32 h_zone) {
	size_t nombre_cadres = 9*nombre_points;
	size_t nb_colones = floor(sqrt((double)nombre_cadres));
	size_t nb_lignes = (nombre_cadres / nb_colones)+1;
	nombre_cadres = nb_lignes*nb_colones;
	bool cases[nombre_cadres];
	for (size_t i = 0; i < nombre_cadres; i++)
	{
		cases[i] = true;
	}
	

	for (size_t i=0; i<nombre_points; ++i) {
		size_t num_cadre = rand()%nombre_cadres;
		while (!cases[num_cadre]) {
			num_cadre++;
			if (num_cadre == nombre_cadres) {
				num_cadre = 0;
			}
		}
		
		SDL_Rect rect_cadre = create_rect_cadre(num_cadre, nb_colones, nb_lignes, w_zone, h_zone);
		points[i].x = rect_cadre.x + rand()%rect_cadre.w;
		points[i].y = rect_cadre.y + rand()%rect_cadre.h;
		points[i].val = i;
	
		// Ligne precedente
		if (num_cadre >= nb_colones) {
			const size_t rang = num_cadre-nb_colones;
			cases[rang] = false;
			if (rang%nb_colones!=0) {
				cases[rang -1] = false;
			}
			if ((rang+1)%nb_colones!=0) {
				cases[rang+1] = false;
			}
		}
		// Ligne actuelle
		cases[num_cadre] = false;
		if (num_cadre%nb_colones!=0) {
			cases[num_cadre-1] = false;
		}
		if ((num_cadre+1)%nb_colones!=0) {
			cases[num_cadre+1] = false;
		}
		// Ligne suivante
		if (nombre_cadres - num_cadre >= nb_colones) {
			const size_t rang = num_cadre+nb_colones;

			cases[rang] = false;
			if (rang%nb_colones!=0) {
				cases[rang -1] = false;
			}
			if ((rang+1)%nb_colones!=0) {
				cases[rang+1] = false;
			}
		}
	}
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
	
	for (size_t i = 0; i < nombre_noeuds; i++) {
		for (size_t j = 0; j < nombre_noeuds; j++) {
			dist[i][j] = graph[i][j];
		}
	}
	for (size_t k = 0; k < nombre_noeuds; k++) {
		for (size_t i = 0; i < nombre_noeuds; i++) {
			for (size_t j = 0; j < nombre_noeuds; j++) {
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
	fisherYatesMelange(&chemin);

	return chemin;
}

int calculDistanceGrapheComplet(const Matrice matrice, const Chemin* chemin) {
	int longeur = 0;
	for (size_t i=1; i<chemin->nombre_elems; ++i) {
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


int exist(int list[],int i,const size_t nombre_points)
{	
	int j;
	int present=0;
	for(j=0;j<nombre_points;j++)
	{
		if (list[j]==i)
		{
			present=1;
		}
	}
	return present;

}

int Glouton_sans_proba(Matrice dist,const size_t nombre_points)
{
	int i;
	int i_temp = 0;
	
	int list[nombre_points+1];
	int distance=INT16_MAX;
	int distance_mini = 0;
	list[0] = rand() % nombre_points;
	for (int compt=1; compt<nombre_points; ++compt)
	{
		distance=INT16_MAX;
		for (i=0;i<nombre_points;i++)
		{
			if (list[compt-1]!=i && dist[list[compt-1]][i]<distance && exist(list,i,compt)==0)
			{
				distance =dist[list[compt-1]][i];
				i_temp=i;
			}
		}
		distance_mini=distance_mini+distance;
		list[compt]=i_temp;
	}

	list[nombre_points] = list[0];
	distance_mini += dist[list[nombre_points-1]][list[nombre_points]];
	
	return distance_mini;
}

int Glouton_avec_proba(void * params)
{
	int i;
	int i_temp = 0;
	struct params *myParams = (struct params *)params;
   
   	Matrice dist = myParams->dist;
   	size_t nombre_points = myParams->nombre_points;
   	float p = myParams->p;
	
	int list[nombre_points+1];
	int distance=INT16_MAX;
	int distance_mini = 0;
	list[0] = rand() % nombre_points;
	for (int compt=1; compt<nombre_points; ++compt)
	{
		distance=INT16_MAX;
		for (i=0;i<nombre_points;i++)
		{
			int proba_passer = rand() / RAND_MAX;
			if(dist[list[compt-1]][i]>distance && p < proba_passer)
			{
				distance =dist[list[compt-1]][i];
				i_temp=i;
			}
			else if (list[compt-1]!=i && dist[list[compt-1]][i]<distance && exist(list,i,compt)==0)
			{
				distance =dist[list[compt-1]][i];
				i_temp=i;
			}
		}
		distance_mini=distance_mini+distance;
		list[compt]=i_temp;
	}

	list[nombre_points] = list[0];
	distance_mini += dist[list[nombre_points-1]][list[nombre_points]];
	
	return distance_mini;
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

double init_temperature(Matrice matrice, size_t nombre_noeud) {
	Chemin chemin = generer_solution_initiale(nombre_noeud);
	int longeur_max = calculDistanceGrapheComplet(matrice, &chemin);
	for (size_t i=0; i<20; ++i) {
		fisherYatesMelange(&chemin);
		int longeur = calculDistanceGrapheComplet(matrice, &chemin);
		if (longeur > longeur_max) {
			longeur_max = longeur;
		}
	}

	return longeur_max;
}

void fisherYatesMelange(Chemin* chemin)
{
	for (int i = 0; i < chemin->nombre_elems-1; i++)
	{
		int j = rand() % (i +1);
		int temp = chemin->val[i];
		chemin->val[i] = chemin->val[j];
		chemin->val[j] = temp;
	}
	chemin->val[chemin->nombre_elems-1] = chemin->val[0];

}

int recuit(Matrice matrice, int N, int nombre_iterations) {
	if (N <= 1) {
		return 0;
	}

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

