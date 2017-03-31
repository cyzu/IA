#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int max(int a, int b){
	if (a < b) return b;
	else return a;
}

int min(int a, int b){
	if (a < b) return a;
	else return b;
}

int difference(int a, int b){
	int m, n;
	m = max(a, b);
	n = min(a, b);
	
	return (m-n);
}

/* fonction qui renvoie le numéro de la variable ayant le minimum de valeur */
int domaine_min(Csp *csp){
    int i, j;
    int compteur = 0, dom_min = csp->nb_valeurs, var_min = 0;
    for (i = 0; i < csp->nb_variables; i++) {
        for (j = 0; j < csp->nb_valeurs; j++) {
            if (csp->domaines[i][j] > -1) compteur++;
        }
        if (dom_min > compteur){
            dom_min = compteur;
            var_min = i;
        }
    }
    return var_min;
}
