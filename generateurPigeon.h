#ifndef __GENERATEUR_P_
#define __GENERATEUR_P_

#include "csp.h"

void init_csp_Pigeon(Csp *csp);
void generateur_Pigeon (Csp *csp,  int nombre);
void filtrage_pigeon(Csp *csp, int var_val[], int domaines_disponibles[], int variable, int domaine, int tour);

#endif
