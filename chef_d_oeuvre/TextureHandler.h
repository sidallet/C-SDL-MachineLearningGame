/**
 * \file TextureHandler.h
 * \brief Define a structure, an enum and associated functions to handle textures loading, accessing and freeing
 * \author Martin Rouault
 * \date 16/12/2021
 */

#ifndef TEXTURE_HANDLER_INCLUDED
#define TEXTURE_HANDLER_INCLUDED

#include <SDL2/SDL.h>
#include <stdio.h>
#include "TextureLoader.h"

/** List here the different texture to handle 
 * In the enum
 *		Island become TEXTURE_Island
 * In file
 *		Island become Island.png
 * \sa MACRO_TEXTURE_ENUM_GEN(E)
 */
#define MACRO_FOR_ALL_TEXTURES(M) 	\
	M(Bus) \
	M(Dragster) \
	M(BusViolet) \
	M(BusVert) \
	M(Route) \
	M(voiture_course) \
	M(Coeur_gris) \
	M(Coeur_rouge)\
	M(Piece)

/**
 *  \brief Allow the generation of enum
 * 	\sa MACRO_FOR_ALL_TEXTURES(M)
 */
#define MACRO_TEXTURE_ENUM_GEN(E) TEXTURE_##E,

/**
 * \enum EnumTextures
 * \brief Indexes for textures in TextureHandler
 */
typedef enum
{
	MACRO_FOR_ALL_TEXTURES(MACRO_TEXTURE_ENUM_GEN) 
	NB_TEXTURES_DEFINED ///< Contains the number of textures
} EnumTextures;


/**
 * \struct TextureHandler
 * \brief A struct to handle all textures
 */
typedef struct
{
	SDL_Texture* textures[NB_TEXTURES_DEFINED]; ///< An array of texture (indexed with EnumTextures)
} TextureHandler;


/**
 * \brief Create a TextureHandler and load each texture defined in MACRO_FOR_ALL_TEXTURES in it
 * \param renderer The renderer which is passed to texture when they are created
 * \return TextureHandler filled with textures
 */
TextureHandler newTextureHandler(SDL_Renderer* renderer);

/**
 * @brief Free every textures in the TextureHandler
 * @param textureHandler The texturehandler containing textures to free (textures array will be filled with NULL after calling this function)
 */
void freeTextureHandler(TextureHandler* textureHandler);

#endif // TEXTURE_HANDLER_INCLUDED
