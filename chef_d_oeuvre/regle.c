#include "regle.h"
#include <stdio.h>

Regle chargerRegle(FILE* flux) {
	Regle regle;
	fscanf(flux, "[%d %d %d %d %d] -> %d (%d)", 
			&regle.observ.routes[0],
			&regle.observ.routes[1],
			&regle.observ.routes[2],
			&regle.observ.routes[3],
			&regle.observ.routes[4],
			(int*)&regle.decis,
			&regle.priorite
	);	

	return regle;
}

TabRegle chargerTabRegle(FILE* flux) {
    TabRegle tabRegle;
    for (int i = 0; i < NB_REGLES; i++)
    {
        Regle regle;
        regle = chargerRegle(flux);
		fscanf(flux, "\n");
        tabRegle.regles[i] = regle;
    }
	return tabRegle;
}


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
    fprintf(flux, " -> %d (%d)\n",regle.decis, regle.priorite);
}

void afficherTabRegle(FILE* flux, const TabRegle tabRegle)
{
    for (int i = 0; i < NB_REGLES; i++)
    {
        afficherRegle(flux,tabRegle.regles[i]);
        fprintf(flux,"\n");
    }
    
}


