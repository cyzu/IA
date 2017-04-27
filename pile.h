#ifndef H_PILE
#define H_PILE

#define MAX_PILE    100

typedef struct Element {
    int variable;
    int valeur;
    struct Element *suivant;
} Element;

typedef struct {
    Element *p;
} Pile;


Pile *init_pile();
int peek(Pile *pile);
void push(Pile *pile, int var, int val);
void pop(Pile *pile);
int isEmpty(Pile *pile);
void afficherPile(Pile *pile, const char *c);
void copier(Pile *pile1, Pile *pile2);

#endif
