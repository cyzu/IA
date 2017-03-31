#ifndef __CSP_
#define __CSP_

#define MAX_VARIABLES 100
#define MAX_RELATIONS 50
#define MAX_DOMAINES 50


typedef struct {
	int relation[MAX_RELATIONS][MAX_RELATIONS];
}Relations;

typedef struct{
	int nb_variables;
	int nb_valeurs;
	int domaines[MAX_VARIABLES][MAX_DOMAINES];
	Relations *contraintes[MAX_VARIABLES][MAX_VARIABLES];
}Csp;

void init_tables_disponibles (Csp *csp, int domaines_disponibles[], int variables_disponibles[]);
int domaines_libres(int dom[]);
int trouver_valeur(Csp *csp, int var);
int verification_contraintes(Csp *csp, int var_val[], int variable, int domaine);

void affichage_domaines(Csp *csp);
void Forward_Checking (Csp *csp);
int main ();

#endif
