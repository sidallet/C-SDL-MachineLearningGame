#include <assert.h>
#include <stdbool.h>
#include "wrapper_sdl.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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


int main()
{
    int N = 10; 
    int** matrice = (int**)malloc(N * sizeof(int*));
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Rect rect = {0,0,1000,800};
    

    if (!start_sdl(&window,&renderer, "Space Game", &rect))
    {
        return EXIT_FAILURE;
    }

    for (int i = 0; i < N; i++) { //init matrice
        matrice[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            matrice[i][j] = 0;
        }
    }//fin init
    genereMatriceArbre(matrice, 0, N-1);

    //affichage matrice
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrice[i][j]);
        }
        printf("\n");
    }
    
    //lib de la mémoire
    for (int i = 0; i < N; i++) {
        free(matrice[i]);
    }
    free(matrice);

    clean_sdl(&window,&renderer);

    return 0;
}





