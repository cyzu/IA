#ifndef __PILE_
#define __PILE_

#define MAX_PILE 200

int sommet_courant;

typedef struct{
	int variable, valeur;	
}Pile;

typedef struct {
	Pile p[MAX_PILE];
} _Pile;

void init_pile(_Pile solutions[]);
void push(Pile pile[], int var, int val);
Pile pop(Pile pile[]);
void afficher(Pile pile[],int k, const char c[]);
void copier(Pile pile1[], Pile pile2[]);

#endif
