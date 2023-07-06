#ifndef REGLE_H
#define REGLE_H
#include <stdio.h>

//------------------------------------------------------------------

typedef enum {
        JOKER=-1, VIDE, LOIN, MOYEN, PROCHE, CONTACT,  // mur ?
}EtatRoute;
#define NB_ETAT 6
typedef struct 
{
    EtatRoute routes[5]; //Gauchegauche, gauche, centre, droite, droitedroite
}Observation;

//------------------------------------------------------------------

typedef struct 
{
    EtatRoute presence;
    int colonne; //-2 à 2 (relatif à la voiture)
}ObservationPiece;


typedef enum {
    GAUCHE=-1, RIEN, DROITE
} Decision ;

typedef struct {
    Observation observ;
    ObservationPiece obsPiece;
    Decision decis;
    int priorite;
}Regle;

#define NB_REGLES 20
typedef struct 
{
    Regle regles[NB_REGLES]; 
}TabRegle;

void afficherRegle(FILE* flux, const Regle regle);
void afficherObservation(FILE* flux, const Observation observation);
void afficherObservationPiece(FILE* flux, const ObservationPiece obsPiece);

Regle chargerRegle(FILE* flux);
TabRegle chargerTabRegle(FILE* flux);
void afficherTabRegle(FILE* flux, const TabRegle tabRegle);

Regle genererRegleAlea();
TabRegle alterTabRegle(TabRegle tabRegle);

#endif //REGLE_H
