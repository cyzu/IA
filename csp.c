#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "csp.h"
#include "generateurPigeon.h"
#include "generateurDame.h"
#include "util.h"
#include "pile.h"


Pile solutions[MAX_PILE];
Pile *pile;
int flag_pigeon = 1;
int flag_all = 0;
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

/* fonction qui verifie toutes les contraintes en relation avec variable
 * renvoie renvoie le numéro de l'autre variable avec qui viole une contrainte, -1 sinon */
int verification_contraintes(Csp *csp, int var_val[], int variable, int domaine){
    int i;
    for (i = variable; i > -1; i--) {
        if (var_val[i] > -1){
            if (csp->contraintes[i][variable] != NULL){
                if (csp->contraintes[i][variable]->relation[var_val[i]][domaine] == -1){
					return i;
				}
            }
        }
    }
    return -1;
}

void affichage_varval(int varval[]){
    int i;
    printf("varval : ");
    for (i = 0; i < 10; i++) {
        printf("[%d]=%d, ", i, varval[i]);
    }
    printf("\n");
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

void Backtrack(Csp *csp, int var_val[], int var_courante, int domaine_courant, int tour){
	int i, j;
	
    var_val[var_courante] = -1;
	pop(pile);
	
	for (i = var_courante; i < csp->nb_variables; i++) {
		for (j = 0; j < csp->nb_valeurs; j++) {
			if (csp->domaines[i][j] == -tour && i != 0) {
				csp->domaines[i][j] = 1;
			}
		}
	}
	if (domaine_courant == -1) return;
	
	/* si c'est la première ligne ou pas */
	if (tour > 1) csp->domaines[var_courante][domaine_courant]  = -tour + 1;
	else csp->domaines[var_courante][domaine_courant] = -tour;
    
}

void Forward_Checking (Csp *csp){
    int var_courante = 0, domaine_courant = 0, tour = 1;
    int domaines_disponibles[MAX_DOMAINES], var_val[MAX_VARIABLES];
    int i;
    
    pile = init_pile();
    init_tables_disponibles(csp, domaines_disponibles, var_val);
    
    do {
		while(var_courante < csp->nb_variables && trouver_valeur(csp, 0) != -1){
			//~ printf("\n------TOUR %d------\n", tour);

			domaine_courant = trouver_valeur(csp, var_courante);

			/* si il n'y a plus de domaine ou ne verifie pas une contrainte */
			if (domaine_courant == -1 || domaines_libres(domaines_disponibles) == -1) {
                
					tour--;
                    var_courante--;
					domaine_courant = var_val[var_courante];
					
					Backtrack(csp, var_val, var_courante, domaine_courant, tour);
					
					domaines_disponibles[domaine_courant] = 1;
			} //fin if plus de domaine ou contrainte fausse
			else {
				if (flag_pigeon == 1) filtrage_pigeon(csp, var_val, domaines_disponibles,var_courante, domaine_courant, tour);
				else filtrage_dame(csp, var_val, domaines_disponibles, var_courante, domaine_courant, tour);
				
                tour++;
                var_courante++;
			}
			//~ affichage_domaines(csp);
			//~ afficherPile(pile, "**pile");
		} //fin while
		
		copier(pile, &solutions[cmpt_sol]);
		/* backtrack de un niveau pour pouvoir continuer*/
		if (trouver_valeur(csp, 0)==-1 || flag_all == 0) flag_all = 0;
        else {
           		tour--;
		    	var_courante--;
				cmpt_sol ++;
			
		    	domaine_courant = var_val[var_courante];
                Backtrack(csp, var_val, var_courante, domaine_courant, tour);
        	}
    } while (flag_all == 1);
    
    if (!solutions[0].p){
        cmpt_sol = -1;
        return;
    }
}

void Back_Jumping(Csp *csp){
	int var_courante = 0, domaine_courant = 0, var_contrainte = -1;
	int domaines_disponibles[MAX_DOMAINES],var_val[MAX_DOMAINES], tour = 1;
	int i ,j, k;
	
    pile = init_pile();
	init_tables_disponibles(csp, domaines_disponibles, var_val);
	
	do {
	
		while (var_courante < csp->nb_variables && trouver_valeur(csp, 0) != -1){
//            printf("\n------TOUR %d------\n", tour);
			domaine_courant = trouver_valeur(csp, var_courante);
//            printf("var %d | domaine %d\n", var_courante, domaine_courant);
			if (domaine_courant == -1){
//                printf("pas de domaine (-1)..\n");
				tour --;
				var_courante --;
				
				domaine_courant = var_val[var_courante];
				var_val[var_courante] = -1;
				pop(pile);
				
				for (i = var_courante; i < csp->nb_variables; i++) {
					for (j = 0; j < csp->nb_valeurs; j++) {
						if (csp->domaines[i][j] < -tour && i != 0) {
							csp->domaines[i][j] = 1;
						}
					}
				}
				/* si c'est la première ligne ou pas */
				if (tour > 1) csp->domaines[var_courante][domaine_courant]  = -tour;
				else csp->domaines[var_courante][domaine_courant] = -tour;	
			}
			else {
				var_contrainte  = verification_contraintes(csp, var_val, var_courante, domaine_courant);
				if (var_contrainte != -1){
//                    printf("contrainte violée avec var %d\n", var_contrainte);
					/* il y a une contrainte violée */
					if (trouver_valeur(csp, var_courante) != -1) {
						csp->domaines[var_courante][domaine_courant] = -tour;
					}
					else {
						
						for (i = var_courante; i >= var_contrainte; i--){
							var_val[i] = -1;
							pop (pile);
							
							for (j = var_courante; j < csp->nb_variables; j++) {
								for (k = 0; k < csp->nb_valeurs; k++) {
									if (csp->domaines[j][k] < -var_contrainte && j != 0) {
										csp->domaines[j][k] = 1;
									}
								}
							}
						}
						var_courante = var_contrainte;
						tour = var_courante - var_contrainte;
						
						if (tour > 1) csp->domaines[var_courante][domaine_courant]  = -tour + 1;
						else csp->domaines[var_courante][domaine_courant] = -tour;
					}
				} 
				else {
//                    printf("push, ok!\n");
					push (pile, var_courante, domaine_courant);
					
					var_val[var_courante] = domaine_courant;
					domaines_disponibles[domaine_courant] = 0;
					
					tour ++;
					var_courante ++;
				}
			}
//            affichage_domaines(csp);
//            affichage_varval(var_val);
//            afficherPile(pile, "");
		}//fin while
		
        copier(pile, &solutions[cmpt_sol]);
        /* backtrack de un niveau pour pouvoir continuer*/
        if (trouver_valeur(csp, 0)==-1 || flag_all == 0) flag_all = 0;
        else {
            tour--;
            var_courante--;
            cmpt_sol ++;
            
            domaine_courant = var_val[var_courante];
            Backtrack(csp, var_val, var_courante, domaine_courant, tour);
        }
    } while (flag_all == 1);
    
    if (!solutions[0].p){
        cmpt_sol = -1;
        return;
    }
}

int main (){
	Csp csp;
	int nombre, algo, i, flag_affichage;
    
    float temps_exec;
    clock_t debut, fin;
    
	printf("Quel problème résoudre ?\n	0-le problème des Dames\n	1-le problème des Pigeons     \n");
	scanf("%d", &flag_pigeon);
	printf("Quel algorithme utiliser ?\n	0-Forward Checking\n	1-Backjumping     \n");
	scanf("%d",&algo);
	printf("Travailler avec combien de variable ?   ");
	scanf("%d", &nombre);
	printf("Voulez-vous rechercher une ou toutes les solution ?\n	0-une solution\n	1-toutes les solutions     \n");
	scanf("%d", &flag_all);
	printf("\n");
	if (flag_pigeon == 1){
		init_csp_Pigeon(&csp);
		generateur_Pigeon(&csp, nombre);
	}
	else {
		init_csp_Dame(&csp);
		generateur_Dame(&csp, nombre);
	}
    
    
    for (i = 0; i < MAX_PILE; i++) solutions[i] = *init_pile();
    
    debut = clock();
    
	if (algo == 0) Forward_Checking(&csp);
	else Back_Jumping(&csp);

    fin = clock();
    temps_exec = (float)(fin-debut)/60;
   
	printf("-------------------------------------------\n");
	if (cmpt_sol == -1) printf("Il n'y a pas de solution.\n");
	else if (cmpt_sol == 0) {
		printf("Il y a une solution.\n");
		cmpt_sol = 1;
	}
	else printf("Il y a %d solutions.\n", cmpt_sol);
    printf("Temps d'éxectution de l'algorithme : %f ms\n", temps_exec);
    printf("-------------------------------------------\n");
    
    if (cmpt_sol >= 0) {
		printf("\nAfficher le(s) solution(s)?\n	0-oui\n	1-non\n");
		scanf("%d",&flag_affichage);
		
		if (flag_affichage == 0) for (i = 0; i < cmpt_sol; i++) afficherPile(&solutions[i], "***"); 
	}
    
    
	return 0;
}
