#include <stdio.h>
#include <stdlib.h>
#include "generateurPigeon.h"
#include "pile.h"

//extern Pile pile[MAX_PILE];
extern _Pile solutions[MAX_PILE];
extern int cmpt_sol;

/** Initialisation des tableaux de CSP (tous à 0) **/
Csp init_csp_Pigeon (Relations *rel){
    int i,j;
    Csp csp;
    
    csp.nb_variables = 0;
    csp.nb_valeurs = 0;
    
    /** initialiser les relations à 0 */
    for (i = 0; i < MAX_RELATIONS; i++){
        for (j = 0; j < MAX_RELATIONS; j++) rel->relation[i][j] = 0;
    }
    
    /** si i==j, il n'y a pas de contraintes donc je mets les contraintes à NULL */
    for (i = 0; i < MAX_VARIABLES; i++) {
        for (j = 0; j < MAX_VARIABLES; j++){
            if (i != j) csp.contraintes[i][j] = rel;
            else csp.contraintes[i][j] = NULL;
        }	
    }
    return csp;
}

void generateur_Pigeon (Csp *csp, Relations *rel, int nombre){
	int i = 0, j = 0, k = 0, l = 0;
	
	FILE* file = fopen("generateur_pigeon.txt", "w+");
	if (file == NULL) {
		printf("echec création fichier\n"); 
		exit(1);
	}
	
	/** initialisation variables et valeurs */
	csp->nb_variables = nombre;
	csp->nb_valeurs = nombre - 1;
	
	
	/** initialisation des domaines */
	fprintf(file, "générateur de pigeons : %d pigeons\n\nvariable : domaines\n", nombre);
	for (i = 0; i < nombre; i++){
		fprintf(file, "	%d : {", i);
		for (j = 0; j < nombre -1; j++){
			csp->domaines[i][j] = 1;
			fprintf(file, "%d ", j);
		}
		fprintf(file, "}\n");
	}
	
	
	/** initialisation des relations : 2 variables peuvent s'affecter à 2 valeurs différentes (i!=j) */
	for (i = 0; i < nombre-1; i++){
		for (j = 0; j < nombre-1; j++){
			if (i != j) rel->relation[i][j] = 1;
			else rel->relation[i][j] = 0;
		}
	}
	
	/** 
	 * initialisation des relations (toutes les possibilités d'affectations entre 2 variables)
	 * contraintes[VARIABLE][VARIABLE].relations[VALEUR var1][VALEUR var2]
	 * le relation est le même pour toutes les contraintes ici.
	 * */	 
	 fprintf(file, "\ncontraintes (variable 1, variable 2) : tuples possibles\n");
	 for (i = 0; i < nombre; i++) {
		 for (j = 0; j < nombre; j++){
			 if (i != j){
				 csp->contraintes[i][j] = rel;
				 fprintf(file, "	(%d, %d) : ", i, j);
				 for (k = 0; k < nombre -1; k++){
					 for (l = 0; l < nombre -1; l++){
						 if (csp->contraintes[i][j]->relation[k][l] == 1)
							fprintf(file, "(%d, %d) ",k,l);
					 }
				 }
				 fprintf(file, "\n");
			 }
	 	 }	
	 }
	fclose(file);
}


/* fonction qui affecte à la variable, le domaine choisi puis filtre les domaines */
void filtrage_pigeon(Csp *csp, int var_val[], int domaines_disponibles[], int variable, int domaine, int tour){
    int k;
    push(solutions[cmpt_sol].p, variable, domaine);
    var_val[variable] = domaine;
    
    domaines_disponibles[domaine] = domaines_disponibles[domaine] - tour -1;
    for (k = variable+1; k < csp->nb_variables; k++)
        csp->domaines[k][domaine] = csp->domaines[k][domaine] - tour -1;
}
