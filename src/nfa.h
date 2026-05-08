#ifndef NFA_H
#define NFA_H
#include "ast.h"

typedef struct etat {
    int no;
    int symbole;
    struct Etat *out1, *out2;    
} Etat;

typedef struct Nfa{
    NfaType type;
    char value;           
    struct Nfa *gauche;    
    struct Nfa *droit;
} Nfa;

void graphe_nfa(Nfa *automate);

#endif 