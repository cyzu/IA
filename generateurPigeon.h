#ifndef __GENERATEUR_P_
#define __GENERATEUR_P_

#include "csp.h"

Csp init_csp_Pigeon(Relations *rel);
void generateur_Pigeon (Csp *csp, Relations *rel, int nombre);
void filtrage_pigeon(Csp *csp, int var_val[], int domaines_disponibles[], int variable, int domaine, int tour);

#endif
