#ifndef NFA_H
#define NFA_H
#include "ast.h"
#define EPSILON -1 // defini un caractere appelé epsilon qui a pour valeur -1

typedef struct Etat {
    int no;
    int symbole;
    struct Etat *out1, *out2;    
} Etat;

typedef struct Nfa{
    Etat *debut;
    Etat *fin;
} Nfa;

Etat* creer_etat(int symbole, Etat *out1, Etat *out2);
Nfa creer_nfa_char(char c);
Nfa union_nfa(Nfa *a1, Nfa *a2);
Nfa concat_nfa(Nfa *gauche, Nfa *droit);
Nfa etoile_nfa(Nfa *a);
Nfa ast_to_nfa(Ast *arbre);
void graphe_nfa(Nfa *automate);
int get_nb_etats_totals()

#endif 