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
    GAUCHE=-1, RIEN, DROITE
} Decision ;

typedef struct {
    Observation observ;
    Decision decis;
    int priorite;
}Regle;

#define NB_REGLES 12
typedef struct 
{
    Regle regles[NB_REGLES]; 
}TabRegle;

void afficherRegle(FILE* flux, const Regle regle);
void afficherObservation(FILE* flux, const Observation observation);

Regle chargerRegle(FILE* flux);
TabRegle chargerTabRegle(FILE* flux);
void afficherTabRegle(FILE* flux, const TabRegle tabRegle);


#endif //REGLE_H
