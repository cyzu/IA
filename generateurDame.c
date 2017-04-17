#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "generateurDame.h"
#include "pile.h"


extern Pile solutions[MAX_PILE];
extern Pile *pile;
extern int cmpt_sol;

/** Initialisation de relation : cette fonction créée une nouvelle 
 * matrice Relations à chaque appel */
Relations* init_relation(){
    Relations rel;
    int i, j;
    for (i = 0; i < MAX_RELATIONS; i++){
		for (j = 0; j < MAX_RELATIONS; j++){
			rel.relation[i][j] = -1;
		}
	} 
    return &rel;
 }

/** Initialisation des tableaux de CSP (tous à vide) **/
void init_csp_Dame(Csp *csp){
    int i,j;
    csp->nb_variables = 0;
    csp->nb_valeurs = 0;
    
    /** si i==j, il n'y a pas de contraintes donc 
     * je mets les contraintes à NULL */
    for (i = 0; i < MAX_VARIABLES; i++) {
            for (j = 0; j < MAX_VARIABLES; j++){
            if (i != j) csp->contraintes[i][j] = init_relation();
            else csp->contraintes[i][j] = NULL;
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
		fprintf(file, "	%d : {", i);
		for (j = 0; j < nombre; j++){
			csp->domaines[i][j] = 1;
			fprintf(file, "%d ", j);
		}
		fprintf(file, "}\n");
	}
	
	/** le domaine[k][l] == 1 lorsque 2 variables peuvent prendre 
	 * les valeurs k et l respectivement */
	fprintf(file, "\ncontraintes (variable 1, variable 2) : tuples possibles\n");
	for (i = 0; i < nombre; i++){
		for (j = 0; j < nombre; j++){
            
			/* Si il y a une contrainte entre i et j */
			if (csp->contraintes[i][j] != NULL){
				fprintf(file, "	(%d, %d) : ", i, j);
				for (k = 0; k < nombre; k++){
					for (l = 0; l < nombre; l++){
						if (difference(i, j)!=difference(k, l) && k!=l){							
							csp->contraintes[i][j]->relation[k][l] = 1;
							fprintf(file, "(%d, %d) ",k,l);
						}
                    }
                }
                fprintf(file, "\n");
            }
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
