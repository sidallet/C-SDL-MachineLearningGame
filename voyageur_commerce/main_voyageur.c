#include <SDL2/SDL_events.h>
#include <SDL2/SDL2_framerate.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "wrapper_sdl.h"
#include "voyageur.h"
#include "matrice.h"

//Permet de dessiner un SDL_Rect
void draw_rec(SDL_Renderer* renderer,int rectWidth, int rectHeight,int rectx,int recty,int Red,int Green,int Blue)
{  
    SDL_Rect rect;
    rect.x = rectx;
    rect.y = recty;
    rect.w = rectWidth;
    rect.h = rectHeight;
    SDL_SetRenderDrawColor(renderer,Red,Green,Blue,200);
    SDL_RenderFillRect(renderer, &rect);

}

/**
 * Permet d'afficher un graphe sur une fenetre
 * renderer : renderer SDL
 * matrice_graphe : matrice du graphe à afficher
 * noeuds : liste des points à afficher
 * nombre_noeuds : nombre de points
 * indicesPointSelect : numero des points cliqués
 * nb_indicesPointSelect : nombre de points cliqués
 **/
void afficherGraphe(SDL_Renderer* renderer, int** matrice_graphe, const Point noeuds[], const size_t nombre_noeuds, int indicesPointSelect[], int nb_indicesPointSelect) {
	for (size_t j = 0; j<nombre_noeuds; ++j) {
		for (size_t i = j; i<nombre_noeuds; ++i) {
			if (matrice_graphe[i][j]) {
				Uint16 hash_red = noeuds[i].x ^ noeuds[j].y*7;
				thickLineRGBA(renderer, noeuds[i].x, noeuds[i].y, noeuds[j].x, noeuds[j].y, 3, hash_red, j*10, 255, 255);
			}
		}
	}
	//Affichage points selectionnés
	for (size_t i=0; i+1<nb_indicesPointSelect; ++i) {
		circleRGBA(renderer, noeuds[indicesPointSelect[i]].x, noeuds[indicesPointSelect[i]].y, RAYON_POINT*1.3, 255,255,255,255);
		thickLineRGBA(renderer, noeuds[indicesPointSelect[i]].x, noeuds[indicesPointSelect[i]].y, noeuds[indicesPointSelect[i+1]].x, noeuds[indicesPointSelect[i+1]].y, 3, 255, 255, 255, 255);
	} 

	//Affichage des points
	for (size_t i=0; i<nombre_noeuds; ++i) {
		filledCircleRGBA(renderer, noeuds[i].x, noeuds[i].y, RAYON_POINT, 25*i,255,0,255);
	} 

	char nom_noeud[50];
	//Affichage du dernier point selectionné
	if(nb_indicesPointSelect > 0)
	{
		//sprintf(nom_noeud, "%d", noeuds[indicesPointSelect[nb_indicesPointSelect-1]].val);
		filledCircleRGBA(renderer, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].x, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].y, RAYON_POINT*1.1, 255,255,255,255);
		//stringRGBA(renderer, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].x-RAYON_POINT/2, noeuds[indicesPointSelect[nb_indicesPointSelect-1]].y-RAYON_POINT/2, nom_noeud, 0, 0, 30, 255);
	}

	//Affichage du numero des points
	for (size_t i=0; i<nombre_noeuds; ++i) {
		sprintf(nom_noeud, "%d", noeuds[i].val);
		stringRGBA(renderer, noeuds[i].x-RAYON_POINT/4, noeuds[i].y-RAYON_POINT/4, nom_noeud, 0, 0, 20, 255);
	} 
}

//Permet de chercher quel point à été cliqué par l'utilisateur
int cherchePointClick(const Point noeuds[], const size_t nombre_noeuds, int x_mouse, int y_mouse)
{
	Point click = {
		.x = x_mouse,
		.y = y_mouse
	};
	for (size_t i = 0; i < nombre_noeuds; i++)
	{
		if(distance_eucli(noeuds[i],click) < RAYON_POINT )
		{
			return i;
		}
	}
	return -1;
	
}

//Permet d'afficher la distance du parcours apres l'avoir complété
void afficheScore(SDL_Renderer* renderer, int distTotal, int distPresqueOptimal)
{
	char valStr[80] = "Score :";
	sprintf(valStr,"Score : %d / %d",distTotal, distPresqueOptimal);
	if (TTF_Init() != 0)
	{
    	fprintf(stderr, "Erreur d'initialisation TTF : %s\n", TTF_GetError()); 
	}
	
	TTF_Font* font = NULL;       
	font = TTF_OpenFont("./fonts/Pacifico.ttf", 32);    
	if (font == NULL) fprintf(stderr, "Erreur d'initialisation TTF : %s\n", TTF_GetError()); 
	SDL_Color color = {255, 255, 255, 255};  

	SDL_Surface * surfaceTexte = TTF_RenderText_Solid(font, valStr, color);
	SDL_Texture * textureTexte = SDL_CreateTextureFromSurface(renderer, surfaceTexte);

	int largeurTexte = surfaceTexte->w;
	int hauteurTexte = surfaceTexte->h;

	SDL_Rect rectDest;
	rectDest.x = 50;
	rectDest.y = 50;
	rectDest.w = largeurTexte;
	rectDest.h = hauteurTexte;


	SDL_RenderCopy(renderer, textureTexte, NULL, &rectDest);   

	SDL_DestroyTexture(textureTexte); 
	SDL_FreeSurface(surfaceTexte);  
}



int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[])
{
	size_t nombre_points = 6;
    if (argc>=2 && sscanf(argv[1], "%ld", &nombre_points)) {}

	float p = 0.1;
	int x_mouse, y_mouse, point_click;
	int distTotal = 0;
    srand(time(NULL));
	bool parcoursOK = false;

	

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Rect rect_fenetre = {0,0,1000,800};
    

    if (!start_sdl(&window,&renderer, "Space Game", &rect_fenetre))
    {
        return EXIT_FAILURE;
    }

	Matrice matrice = initMatrice(nombre_points);
    genereMatriceArbre(matrice, 0, nombre_points-1);
    genereGraphe(matrice,p, nombre_points);

	Point points[nombre_points];

	int indicesPointSelect[nombre_points*4];
	int nb_indicesPointSelect = 0;

	generer_points(points, nombre_points, rect_fenetre.w, rect_fenetre.h);
	printf("Coordonnées des points :\n");
	for (size_t i=0; i<nombre_points; ++i) {
		printf("%ld : (%d, %d)\n", i, points[i].x, points[i].y);
	}

	FPSmanager fps_manager;
	SDL_initFramerate(&fps_manager);
	SDL_setFramerate(&fps_manager, 60);

	[[maybe_unused]]Uint32 delta_time = 0;
	
	Matrice matriceDistance = calculMatriceDistance(matrice, points, nombre_points);
	printf("Matrice de distances (avec inf = %d):\n", INT16_MAX);
	afficheMatrice(matriceDistance, nombre_points);
	Matrice matriceGrapheComplet = floydWarshall(matriceDistance, nombre_points); 
	printf("Matrice de distances graphe complet :\n");
	afficheMatrice(matriceGrapheComplet, nombre_points);

    printf("Glouton : \n");
    int longueur_glouton = Glouton_sans_proba(matriceGrapheComplet,nombre_points);
    printf("Glouton : %d \n", longueur_glouton); 

    printf("Glouton proba: \n");
    longueur_glouton = Glouton_avec_proba(matriceGrapheComplet,nombre_points,0.2); 
    printf("Glouton proba: %d \n", longueur_glouton); 

	int longueur_presque_optimale = recuit(matriceGrapheComplet, nombre_points, 1000);

	bool actif = true;
	while (actif) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					actif = false;
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					switch(event.button.clicks)
					{
					case SDL_BUTTON_LEFT: {
						if(!parcoursOK)
						{
							x_mouse = event.button.x;
                        	y_mouse = event.button.y;
							point_click = cherchePointClick(points, nombre_points, x_mouse, y_mouse);
							if(point_click != -1)
							{
							printf("point click %d \n", point_click);
							if(nb_indicesPointSelect == 0 || matrice[point_click][indicesPointSelect[nb_indicesPointSelect-1]] == 1)
							{
								indicesPointSelect[nb_indicesPointSelect] = point_click;
								nb_indicesPointSelect++;

							}
							else if (nb_indicesPointSelect>0 && point_click == indicesPointSelect[nb_indicesPointSelect-1]){
								nb_indicesPointSelect--;
							}
							parcoursOK = verifParcours(indicesPointSelect, nb_indicesPointSelect, nombre_points);
							printf("parcours %d \n", parcoursOK);

							if(parcoursOK)
							{
								distTotal = calculDistanceGraphe(points, indicesPointSelect,nb_indicesPointSelect);
								printf("Dist total = %d\n",distTotal);
							}
						}
						break;
						}
						
					}
					default:
						break;
					} 
				}

			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		afficherGraphe(renderer, matrice, points, nombre_points, indicesPointSelect,nb_indicesPointSelect);
		if(parcoursOK)
		{
			afficheScore(renderer, distTotal, longueur_presque_optimale);
		}
		SDL_RenderPresent(renderer);
		
		delta_time = SDL_framerateDelay(&fps_manager);
	}

    
    
    libereMatrice(matrice, nombre_points);
    clean_sdl(&window,&renderer);

    return 0;
}





