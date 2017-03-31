#include <stdio.h>
#include <stdlib.h>
#include "pile.h"


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

void push(Pile pile[], int var, int val){
	sommet_courant ++;
	pile[sommet_courant].variable = var;
	pile[sommet_courant].valeur = val;
}

Pile pop(Pile pile[]){
    Pile pile_tmp;
    pile_tmp.variable = pile[sommet_courant].variable;
    pile_tmp.valeur = pile[sommet_courant].valeur;
    
    pile[sommet_courant].variable = -1;
    pile[sommet_courant].valeur = -1;
    sommet_courant --;
    
    return pile_tmp;
}

//~ void vider_pile(Pile pile[]){
    //~ int i;
    //~ for (i = 0; i <= sommet_courant; i++) {
        //~ pile[i].variable = -1;
        //~ pile[i].valeur = -1;
    //~ }
    //~ sommet_courant = -1;
//~ }

/* Teste si la pile est vide : renvoie 1 quand c'est vrai, 0 sinon */
//~ int si_pile_vide(){
    //~ return sommet_courant == -1;
//~ }

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

/*int main(){
    Pile pile[MAX_PILE];
    Pile pile_pop;
    
    init_pile(pile);
    afficher(pile, "Init pile");
    
    push(pile, 1, 1);
    afficher(pile, "push de 1, 1");
    
    push(pile, 2, 22);
    push(pile, 3, 333);
    push(pile, 4, 4);
    afficher(pile, "push x3");
    
    pile_pop = pop(pile);
    afficher(pile, "pop 1");
    
    
    pile_pop = pop(pile);
    pile_pop = pop(pile);
    afficher(pile, "pop x2");
    printf("*** pop_var = %d, pop_val = %d\n", pile_pop.variable, pile_pop.valeur);
    
    return 0;
}*/
