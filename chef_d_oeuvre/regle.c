#include "regle.h"

void afficherObservation(FILE* flux, const Observation observation) {
    fprintf(flux, "[");
    for (int i = 0; i < 5; i++) {
        if(i == 4)
        {
            fprintf(flux, "%d",observation.routes[i]);
        }
        else {
            fprintf(flux, "%d ",observation.routes[i]);
        }
            
    }
    fprintf(flux, "]");
}

void afficherRegle(FILE* flux, const Regle regle)
{
    afficherObservation(flux,regle.observ);
    fprintf(flux, "-> %d (%d)\n",regle.decis, regle.priorite);
}


