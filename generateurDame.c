#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "generateurDame.h"
#include "pile.h"


extern Pile solutions[MAX_PILE];
extern Pile *pile;
extern int cmpt_sol;


/** Initialisation des tableaux de CSP (tous à vide) **/
void init_csp_Dame(Csp *csp){
    int i,j,k,l;
    csp->nb_variables = 0;
    csp->nb_valeurs = 0;

    for (i = 0; i < MAX_RELATIONS; i++){
        for (j = 0; j < MAX_RELATIONS; j++){
            csp->contraintes[i][j] = malloc(MAX_VARIABLES * MAX_VARIABLES * sizeof(int));
        }
    }
}

/** générateur de Dames qui rempli le Csp en fonction de 'nombre' */
void generateur_Dame(Csp *csp, int nombre){
	int i, j, k, l;
	
	FILE* file = fopen("generateur_dames.txt", "w+");
	if (file == NULL) {
		printf("echec création fichier\n"); 
		exit(1);
	}
	
	/** initialisation variables et valeurs */
	csp->nb_variables = nombre;
	csp->nb_valeurs = nombre;
		
		
	/** initialisation des domaines */
	fprintf(file, "générateur de dames : %d dames - grille %dx%d\n\nvariable : domaines\n", nombre, nombre, nombre);
	for (i = 0; i < nombre; i++){
		fprintf(file, "	%d = { ", i);
		for (j = 0; j < nombre; j++){
			csp->domaines[i][j] = 1;
			fprintf(file, "%d ", j);
		}
		fprintf(file, "}\n");
	}
	
	/** le domaine[k][l] == 1 lorsque 2 variables peuvent prendre 
	 * les valeurs k et l respectivement */
	fprintf(file, "\ncontraintes (variable 1, variable 2) : tuples possibles\n");
    for (i = 0; i < nombre; i++) {
        for (j = i+1; j < nombre; j++) {
            fprintf(file, "(%d, %d) : ",i,j);
            for (k = 0; k < nombre; k++) {
                for (l = 0; l < nombre; l++) {
                    if (difference(i,j)!=difference(k,l) && (k!=l)) {
                        csp->contraintes[i][j]->relation[k][l] = 1;
                        fprintf(file, "(%d, %d) ",k,l);
                    }
                    else csp->contraintes[i][j]->relation[k][l] = -1;
                }
            }
            fprintf(file, "\n");
        }
    }
    fclose(file);
}


/** fonction qui affecte le domaine à la variable puis filtre les domaines des autres dames */
void filtrage_dame(Csp *csp, int var_val[], int domaines_disponibles[], int variable, int domaine, int tour){
    int k, i, j;
    
    push(pile, variable, domaine);
    var_val[variable] = domaine;
    domaines_disponibles[domaine] = -tour;
    
    /* filtrage de colonne */
    for (k = variable+1; k < csp->nb_variables; k++){
        if (csp->domaines[k][domaine] == 1){
            csp->domaines[k][domaine] = -tour;
        }
    }
    /*filtrage de diagonales */
    if (domaine == 0){
        k = domaine+1;
        for (i = variable+1; i < csp->nb_variables; i++) {
            if (csp->domaines[i][k] == 1) {
              csp->domaines[i][k] = -tour;
            }
            k++;
        }
    }
    else if (domaine == csp->nb_variables-1){
        k = domaine-1;
        for (i = variable+1; i < csp->nb_valeurs; i++) {
            if (csp->domaines[i][k] == 1) {
                csp->domaines[i][k] = -tour;
            }
            k--;
        }
    }
    else {
        k = domaine+1;
        j = domaine-1;
        for (i = variable+1; i < csp->nb_variables; i++) {
            if (csp->domaines[i][k] == 1) {
                csp->domaines[i][k] = -tour;
            }
            if (csp->domaines[i][j] == 1) {
                csp->domaines[i][j] = -tour;
            }
            k++;
            j--;
        }
    }
}
