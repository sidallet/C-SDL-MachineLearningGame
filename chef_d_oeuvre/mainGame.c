#include "wrapper_sdl.h"
#include <SDL2/SDL2_framerate.h>
#include "game.h"

void affichage(SDL_Renderer* renderer, const Game* game) {
	SDL_RenderClear(renderer);
	game_afficher(game,renderer);
	SDL_RenderPresent(renderer);
}


int main (int argc, char* argv[]) {

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect rect_fenetre = {100, 100, 1000, 800};
	if (!start_sdl(&window, &renderer, "Carcheeez", &rect_fenetre)) {
		return EXIT_FAILURE;
	}

	FPSmanager fpsManager;
	SDL_initFramerate(&fpsManager);
	
	SDL_setFramerate(&fpsManager, 60);
	Uint32 delta_time = 0;

	Game game = new_game(renderer);

	bool actif = true;

	while (actif)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				actif = false;
				break;
			
			default:
				game_handle_event(&game, &event, &rect_fenetre);
				break;
			}
		}
		
		affichage(renderer, &game);


		delta_time = SDL_framerateDelay(&fpsManager);

	}
	
	liberer_game(&game);
	clean_sdl(&window, &renderer);
	return EXIT_SUCCESS;
}
