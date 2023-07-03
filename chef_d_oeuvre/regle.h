#ifndef REGLE_H
#define REGLE_H
#include <stdio.h>

typedef enum {
        JOKER=-1, VIDE, LOIN, PROCHE // mur ?
}EtatRoute;

typedef struct 
{
    EtatRoute routes[5]; //Gauchegauche, gauche, centre, droite, droitedroite
}Observation;


typedef enum {
        GAUCHE, RIEN, DROITE
    } Decision ;

typedef struct {
    Observation observ;
    Decision decis;
    int priorite;
}Regle;

void afficherRegle(FILE* flux, const Regle regle);
void afficherObservation(FILE* flux, const Observation observation);

Regle chargerRegle(FILE* flux);


#endif //REGLE_H