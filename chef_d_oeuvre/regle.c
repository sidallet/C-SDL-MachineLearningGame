#include "regle.h"
#include <stdio.h>
#include <stdlib.h>

Regle chargerRegle(FILE* flux) {
	Regle regle;
	fscanf(flux, "[%d %d %d %d %d][%d %d] -> %d (%d)", 
			&regle.observ.routes[0],
			&regle.observ.routes[1],
			&regle.observ.routes[2],
			&regle.observ.routes[3],
			&regle.observ.routes[4],
            &regle.obsPiece.colonne,
            &regle.obsPiece.presence,
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

void afficherObservationPiece(FILE* flux, const ObservationPiece obsPiece) {
    fprintf(flux, "[%d %d]",obsPiece.colonne,obsPiece.presence);
}

void afficherRegle(FILE* flux, const Regle regle)
{
    afficherObservation(flux,regle.observ);
    afficherObservationPiece(flux,regle.obsPiece);
    fprintf(flux, " -> %d (%d)\n",regle.decis, regle.priorite);
}

void afficherTabRegle(FILE* flux, const TabRegle tabRegle)
{
    for (int i = 0; i < NB_REGLES; i++)
    {
        afficherRegle(flux,tabRegle.regles[i]);
    } 
}



Regle genererRegleAlea()
{
	Regle regle;
	for (int i = 0; i < 5; i++)
	{
		int randRegle = (rand() % NB_ETAT)-1;
		regle.observ.routes[i] = randRegle;
	}

	int randDecis = (rand() % 3)-1;
	regle.decis = randDecis;

	int randPrio = (rand() % 10)+1;
	regle.priorite = randPrio;

	regle.obsPiece.colonne = (rand()%5) -2;
	regle.obsPiece.presence = (rand()% NB_ETAT)-1;

	return regle;
	
}

int rand_dif(int base, int mini, int maxi) {
	int mod = maxi - mini +1;
	int nouveau = (rand()%mod) + mini;
	if (nouveau==base) {
		nouveau = ((nouveau+1-mini)%mod) + mini;
	}
	return nouveau;
}


TabRegle alterTabRegle(TabRegle tabRegle) {
	int iRegle = rand() % NB_REGLES;
	int random_num = rand() % 9;  // Génère un nombre aléatoire entre 0 et 6 

	if (random_num < 5) { //Modif observation
		int iObs = rand() % 5;
		int observ = tabRegle.regles[iRegle].observ.routes[iObs];
		if (observ!=JOKER && rand()%3) {
			tabRegle.regles[iRegle].observ.routes[iObs] = JOKER;
			return tabRegle;
		}
		tabRegle.regles[iRegle].observ.routes[iObs] = rand_dif(observ, -1, 4);
	} else if (random_num == 5) { //Modif deplacement
		tabRegle.regles[iRegle].decis = rand_dif(tabRegle.regles[iRegle].decis, -1, 1);
	} else if (random_num == 6){ //Modif priorité
		tabRegle.regles[iRegle].priorite = rand_dif(tabRegle.regles[iRegle].priorite, 1, 10);
	}
	else if (random_num == 7) {
		tabRegle.regles[iRegle].obsPiece.colonne = rand_dif(tabRegle.regles[iRegle].obsPiece.colonne, -2, 2);
	}
	else {
		if (tabRegle.regles[iRegle].obsPiece.presence!=JOKER && rand()%3) {
			tabRegle.regles[iRegle].obsPiece.presence = JOKER;
		}
		else {
			tabRegle.regles[iRegle].obsPiece.presence = rand_dif(tabRegle.regles[iRegle].obsPiece.presence, -1, 4);
		}
	}

	return tabRegle;
}

Regle copier_regle(Regle regle) {
	Regle r;
	r.priorite = regle.priorite;
	r.decis = regle.decis;
	r.obsPiece = regle.obsPiece;
	
	for (size_t i=0; i<5; ++i) {
		r.observ.routes[i] = regle.observ.routes[i];
	}
	return r;
}

TabRegle copier_tab_regle(TabRegle tab) {
	TabRegle nouv;
	
	for (size_t i=0; i<NB_REGLES; ++i) {
		nouv.regles[i] = copier_regle(tab.regles[i]);	
	}
	return nouv;
}

