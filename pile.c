#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

Pile *init_pile()
{
    Pile *pile = malloc(sizeof(*pile));
    pile->p = NULL;
    return pile;
}

/* retourne l'indice de la variable au sommet de pile */
int peek(Pile *pile){
    int ret = -1;
    
    if (pile == NULL){
        printf("Accès pile impossible pour afficher !\n");
        exit(EXIT_FAILURE);
    }
    if(pile->p != NULL) ret = pile->p->variable;
    return ret;
}

void push(Pile *pile, int var, int val)
{
//    printf("push ?\n");
    Element *new = malloc(sizeof(*new));
    if (pile == NULL || new == NULL) {
        printf("Accès pile impossible en push !\n");
        exit(EXIT_FAILURE);
    }
    new->variable = var;
    new->valeur = val;
    new->suivant = pile->p;
    
    pile->p = new;
}

void pop(Pile *pile)
{
	if (pile == NULL) {
        printf("Accès pile impossible en pop !\n");
        exit(EXIT_FAILURE);
    }
//    printf("pop\n");
    Element *elementDepile = pile->p;
//    if (elementDepile->suivant == NULL) pile->p = NULL;
    else pile->p = elementDepile->suivant;
    free(elementDepile);
}

/* retourne 1 si la pile est vide, 0 sinon */
int isEmpty(Pile *pile){
    if (pile->p != NULL) return 0;
    else return 1;
}

void afficherPile(Pile *pile, const char *c)
{
    Element *tmp = pile->p;
    if (pile == NULL){
        printf("Accès pile impossible pour afficher !\n");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", c);
    while (tmp != NULL){
        printf("[%d %d]\n", tmp->variable, tmp->valeur);
        tmp = tmp->suivant;
    }
    printf("\n");
    free(tmp);
}

/* copie le contenu de la pile 1 dans la pile 2 */
void copier(Pile *pile1, Pile *pile2){
    Element *tmp = pile1->p;
    
    while (tmp != NULL){
		push(pile2, tmp->variable, tmp->valeur);
		tmp = tmp->suivant;
	}
	free(tmp);
}
