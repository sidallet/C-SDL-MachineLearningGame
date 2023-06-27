#ifndef SDL_WRAPPER_INCLUDED
#define SDL_WRAPPER_INCLUDED


#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>  

bool init_sdl();

SDL_Window* construct_window(const char* titre, const SDL_Rect* rect, const Uint32 flags);

SDL_Renderer* construct_renderer(SDL_Window* window);

bool start_sdl(SDL_Window** window, SDL_Renderer** renderer, const char* titre, SDL_Rect* rect);

void clean_sdl(SDL_Window** window, SDL_Renderer** renderer);

#endif //SDL_WRAPPER_INCLUDED

