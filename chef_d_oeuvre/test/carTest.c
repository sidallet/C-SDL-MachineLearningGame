#include <assert.h>
#include <stdio.h>
#include <game.h>
#include <boucle_entrainement_ia.h>

void carTestCase(void) {
	SDL_Rect rect_fenetre = {100, 100, 1000, 800};
    Game game = {
		.voiture = {0,rect_fenetre.h-125,65,110},
		.nbVoiture = 1,
		.vitesse = 100,
		.ecart_obstacles = 15,
		.temps_deplacement = 0,
	};
    game.rect_obstacle = malloc(sizeof(SDL_Rect)*game.nbVoiture);
    game.rect_obstacle[0].h = 110;
    game.rect_obstacle[0].w = 65;
    game.rect_obstacle[0].x = 0;
    game.rect_obstacle[0].y = rect_fenetre.h-100;
    
    Observation obs = creerObservation(&game,game.rect_obstacle,game.nbVoiture,&rect_fenetre);
    assert(obs.routes[2] == CONTACT);
    liberer_game(&game);
}

#ifdef UNIT_TEST

int main(){
    carTestCase();
    
    printf("All tests passed !\n");
    return 0;
}

#endif

