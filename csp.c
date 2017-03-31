#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "csp.h"
#include "generateurPigeon.h"
#include "generateurDame.h"
#include "util.h"
#include "pile.h"


_Pile solutions[MAX_PILE];
int flag_pigeon = 1;
int flag_all = 0;
int flag_heurisique = 1;
int cmpt_sol = 0;


/* fonction qui initialise les tables domaines et variables disponibles à 1 */
void init_tables_disponibles (Csp *csp, int domaines_disponibles[], int var_val[]){
	int i;
	for (i = 0; i < MAX_DOMAINES; i++) {
        if (i < csp->nb_valeurs) domaines_disponibles[i] = 1;
        else domaines_disponibles[i] = 0;
    }
    for (i = 0; i < MAX_VARIABLES; i++){
		var_val[i] = -1;
	}
}

/* fonction qui cherche et renvoie le premier domaine disponible ou -1 si il n'y en a pas */
int domaines_libres(int dom[]){
	int i;
	for (i = 0; i < MAX_DOMAINES; i++){
		if (dom[i] == 1) return i;
	}
	return -1;
}

/* fonction qui renvoie le 1er domaine libre de var.
 * si il ne possède plus de domaine, renvoie -1 */
int trouver_valeur(Csp *csp, int var){
	int i;
	for (i = 0; i < csp->nb_valeurs; i++){
        if (csp->domaines[var][i] == 1) return i;
	}
	return -1;
}

/* fonction booléenne (1-faux, 0-vrai) qui verifie toutes les contraintes en relation avec variable-domaine */
int verification_contraintes(Csp *csp, int var_val[], int variable, int domaine){
    int i;
    for (i = 0; i < csp->nb_variables; i++) {
        if (var_val[i] >= 0){
            if (csp->contraintes[i][variable] != NULL){
                if (csp->contraintes[i][variable]->relation[var_val[i]][domaine] == 0) return 1;
            }
        }
    }
    return 0;
}

/* Fonction qui affiche la grille des domaines pour toutes les variables */
void affichage_domaines(Csp *csp){
    int i, j;
    if (flag_pigeon == 1) printf("\nGrille Pigeons : \n");
    else printf("\nGrille Dame : \n");
	
    for (i = 0; i<csp->nb_variables; i++) {
        printf("variable %d :   ", i);
        for (j = 0; j < csp->nb_valeurs; j++) {
            printf("%d  ", csp->domaines[i][j]);
        }
        printf("\n");
    }
}


/** @todo : option (flag) une solution ou toutes */
void Forward_Checking (Csp *csp){
    int var_courante = 0, domaine_courant, tour = 1, i, j;
    int domaines_disponibles[MAX_DOMAINES], var_val[MAX_VARIABLES];
    
    int h_ordre_var[MAX_VARIABLES], h_var = 0;
    h_ordre_var[0] = 0;
    
    init_tables_disponibles(csp, domaines_disponibles, var_val);
    
    do {
		while(var_courante < csp->nb_variables && trouver_valeur(csp, 0) != -1){
//			printf("---- TOUR %d ----\n", tour);
			domaine_courant = trouver_valeur(csp, var_courante);
            
            
            
			/* si il n'y a plus de domaine ou ne verifie pas une contrainte */
			if (domaine_courant == -1 || verification_contraintes(csp, var_val, var_courante, domaine_courant) == 1
					|| domaines_libres(domaines_disponibles)==-1) {

					tour--;
                    if (flag_heurisique == 1) {
                        h_ordre_var[h_var] = -1;
                        h_var--;
                        var_courante = h_ordre_var[h_var];
                    }
                    else var_courante--;
                
					pop(solutions[cmpt_sol].p);
					domaine_courant = var_val[var_courante];
					var_val[var_courante] = -1;
					for (i = var_courante; i < csp->nb_variables; i++) {
						for (j = 0; j < csp->nb_valeurs; j++) {
							if (csp->domaines[i][j] == -tour && i != 0) {
								csp->domaines[i][j] = 1;
							}
						}
					}
					/* si c'est la première ligne ou pas */
					if (tour > 1) csp->domaines[var_courante][domaine_courant]  = -tour + 1;
					else csp->domaines[var_courante][domaine_courant] = -tour;
					
					domaines_disponibles[domaine_courant] = 1;
			} //fin if plus de domaine ou contrainte fausse
			else {
				if (flag_pigeon == 1) filtrage_pigeon(csp, var_val, domaines_disponibles,var_courante, domaine_courant, tour);
				else filtrage_dame(csp, var_val, domaines_disponibles, var_courante, domaine_courant, tour);
				
                tour++;
                if (flag_heurisique == 1) {
                    var_courante = domaine_min(csp);
                    h_ordre_var[h_var] = var_courante;
                    h_var++;
                }
                else var_courante++;
                printf("prochaine var = %d\n", var_courante);
			}
//			afficher(solutions[cmpt_sol].p, "\nAffichage pile :");
//			affichage_domaines(csp);
//			printf("\n");
		} //fin while	
			
		/* backtrack de un niveau pour pouvoir continuer*/
		if (trouver_valeur(csp, 0)==-1) flag_all = 0;
        	else {
            		tour--;
		    	var_courante--;
		    	cmpt_sol ++;

		    	copier(solutions[cmpt_sol-1].p, solutions[cmpt_sol].p);
		    	pop(solutions[cmpt_sol].p);
		    	domaine_courant = var_val[var_courante];
		    	var_val[var_courante] = -1;

		    	for (i = var_courante; i < csp->nb_variables; i++) {
				for (j = 0; j < csp->nb_valeurs; j++) {
			    		if (csp->domaines[i][j] + tour + 1 == 1 && i != 0) {
						csp->domaines[i][j] = 1;
			    		}
				}
		    	}
            
        	}
    } while (flag_all == 1);
    
    if (solutions[0].p[0].variable == -1){
        printf("Il n'y a pas de solution.\n");
        return;
    }
    printf("les %d solutions sont :\n", cmpt_sol);
    for (i = 0; i < cmpt_sol; i++) afficher(solutions[i].p, csp->nb_variables, "***");
}


int main (){
	Csp csp;
	Relations rel;
	int nombre;
	//int i = 0, j = 0, k = 0, l = 0;
	
	printf("Travailler avec combien de Dame ?   ");
	scanf("%d", &nombre);
	
//	csp = init_csp_Pigeon(&rel);
//	init_pile(solutions);
//	generateur_Pigeon(&csp, &rel, nombre);
	
	
    init_csp_Dame(&csp);
    init_pile(solutions);
    generateur_Dame(&csp, nombre);
    flag_pigeon = 0;

	Forward_Checking(&csp);
	
	/* AFFICHAGE CSP APRÈS INIT
	printf("XX : variables = %d -- valeurs = %d\n", csp.nb_variables, csp.nb_valeurs);
	for (i = 0; i< csp.nb_variables; i++){
		for (j = 0; j < csp.nb_variables; j++){
			if (csp.contraintes[i][j] == NULL) printf("[%d][%d] = %p\n", i, j, csp.contraintes[i][j]);
			else {
				for (k = 0; k < csp.nb_valeurs; k++){
					for (l = 0; l < csp.nb_valeurs; l++){	
						printf("[%d][%d]->[%d][%d] = %d\n", i,j,k,l, csp.contraintes[i][j]->relation[k][l]);
					}
				}
			}
		}
	}*/
    
	/** @todo Réinitialisation du csp */

	return 0;
}
