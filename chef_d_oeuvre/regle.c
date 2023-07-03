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
			&regle.decis,
			&regle.priorite
	);	

	return regle;
}


