#include "TextureHandler.h"
#include <errno.h>

#define MACRO_TO_TEXTURE_NAME(N) #N".png",

const char* texturesNames[] = {
	MACRO_FOR_ALL_TEXTURES(MACRO_TO_TEXTURE_NAME)
};

TextureHandler newTextureHandler(SDL_Renderer* renderer) {
	TextureHandler tH;

	const char directoryPath[] = "img/";

	fprintf(stderr, "Nb textures %d\n", NB_TEXTURES_DEFINED);

	for (size_t i = 0; i<NB_TEXTURES_DEFINED; ++i) {
		char* path = (char*)malloc((strlen(directoryPath)+strlen(texturesNames[i])+1)*sizeof(char));
		if (path == NULL) {
			perror("Malloc newTextureHandler"); exit(errno);
		}
		strcpy(path, directoryPath);
		tH.textures[i] = createTextureFromPath(renderer, strcat(path,texturesNames[i]));
		free(path);
	}

	return tH;
}

void freeTextureHandler(TextureHandler* textureHandler) {
	for (size_t i = 0; i<NB_TEXTURES_DEFINED; ++i) {
		if (textureHandler->textures[i] != NULL) {
			SDL_DestroyTexture(textureHandler->textures[i]);
			textureHandler->textures[i] = NULL;
		}
	}
}
