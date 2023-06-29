#ifndef WRAPPER_SDL_INCLUDED
#define WRAPPER_SDL_INCLUDED

#include <SDL2/SDL.h>
#include <stdbool.h>


bool init_sdl();

SDL_Window* construct_window(const char* titre, const SDL_Rect* rect, const Uint32 flags);

SDL_Renderer* construct_renderer(SDL_Window* window);



bool start_sdl(SDL_Window** window, SDL_Renderer** renderer, const char* titre, SDL_Rect* rect);

void clean_sdl(SDL_Window** window, SDL_Renderer** renderer);

#endif //WRAPPER_SDL_INCLUDED
