#ifndef __GENERATEUR_D_
#define __GENERATEUR_D_

#include "csp.h"

Relations* init_relation();
void init_csp_Dame (Csp *csp);
void generateur_Dame(Csp *csp, int nombre);
void filtrage_dame(Csp *csp, int var_val[], int domaines_disponibles[], int variable, int domaine, int tour);
#endif
