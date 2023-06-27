#include <assert.h>
#include <stdbool.h>
#include "wrapper_sdl.h"
#include "voyageur.h"


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

int main()
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Rect rect = {0,0,1000,800};
    

    if (!start_sdl(&window,&renderer, "Space Game", &rect))
    {
        return EXIT_FAILURE;
    }
    clean_sdl(&window,&renderer);
    return 0;
}

