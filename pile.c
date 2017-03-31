#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

/* Fonction qui initialise les valeurs de la pile en -1 */
void init_pile(_Pile solutions[]){
    int i, j;
	for (i = 0; i < MAX_PILE; i++){
        for (j = 0; j < MAX_PILE; j++) {
            solutions[i].p[j].variable = -1;
            solutions[i].p[j].valeur = -1;
        }
    }
	sommet_courant = -1;
}

/* fonction qui met au sommet de la pile le coupe [var-val] */
void push(Pile pile[], int var, int val){
	sommet_courant ++;
	pile[sommet_courant].variable = var;
	pile[sommet_courant].valeur = val;
}

/* fonction qui renvoie le couple en haut de pile et le supprime de la pile */
Pile pop(Pile pile[]){
    Pile pile_tmp;
    pile_tmp.variable = pile[sommet_courant].variable;
    pile_tmp.valeur = pile[sommet_courant].valeur;
    
    pile[sommet_courant].variable = -1;
    pile[sommet_courant].valeur = -1;
    sommet_courant --;
    
    return pile_tmp;
}

/* fonction qui affiche la pile complète */
void afficher(Pile pile[], int k, const char c[]){
    int i;
    printf("%s\n", c);
    for (i = 0; i < k; i++) printf("[%d %d]\n", pile[i].variable, pile[i].valeur);
}

/* cette fonction copie le contenu de la pile 1 dans la pile 2 */
void copier(Pile pile1[], Pile pile2[]){
    int i;
    for (i = 0; i <= sommet_courant; i++) {
        pile2[i].variable = pile1[i].variable;
        pile2[i].valeur = pile1[i].valeur;
    }
}
