/**
 * \file TextureLoader.h
 * \brief Define functions to load textures
 * \author Érina Point
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/*
 * \brief Create a texture with an image
 * \param[in]	renderer	The renderer which will draw the image
 * \param[in]	path	Image's path on the computer
 * \return Return texture created on success. Else return NULL and print the error on STDERR.
*/
SDL_Texture* createTextureFromPath(SDL_Renderer* renderer, char* path);