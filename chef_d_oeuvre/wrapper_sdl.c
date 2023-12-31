#include "wrapper_sdl.h"
#include <assert.h>


bool init_sdl() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                fprintf(stderr, "Erreur init sdl : %s\n", SDL_GetError());
                return false;
        }
        return true;
}

SDL_Window* construct_window(const char* titre, const SDL_Rect* rect, const Uint32 flags) {
        SDL_Window* window = SDL_CreateWindow(titre, rect->x, rect->y, rect->w, rect->h, flags);
        if (window==NULL) {
                fprintf(stderr, "Erreur create window sdl : %s\n", SDL_GetError());
        }

        return window;
}

SDL_Renderer* construct_renderer(SDL_Window* window) {
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        if (renderer==NULL) {
                fprintf(stderr, "Erreur create renderer sdl : %s\n", SDL_GetError());
        }
        return renderer;
}

bool start_sdl(SDL_Window** window, SDL_Renderer** renderer, const char* titre, SDL_Rect* rect) {
        assert(window != NULL && renderer != NULL && "Parametre de sortie nulles");

        if (!init_sdl()) {
                return false;
        }

        *window = construct_window(titre, rect, SDL_WINDOW_RESIZABLE);
        if (*window==NULL) {
                SDL_Quit();
                return false;
        }

        *renderer = construct_renderer(*window);
        if (*renderer==NULL) {
                SDL_DestroyWindow(*window);
                SDL_Quit();
                return false;
        }
        return true;
}

void clean_sdl(SDL_Window** window, SDL_Renderer** renderer) {
    SDL_DestroyRenderer(*renderer);
    SDL_DestroyWindow(*window);
    *window = NULL;
    *renderer = NULL;
    SDL_Quit();
}



