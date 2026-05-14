#ifndef DFA_H
#define DFA_H
#include "nfa.h"

typedef struct TransitionDFA {
    char lettre;                            
    struct Ensemble_etats *destination;
    struct TransitionDFA *suivante;
} TransitionDFA;

//pour rassembler les etats qui correspondent à la meme transition en un meme etat
typedef struct Ensemble_etats{
int no;
int *etats_nfa;
int est_final;
int nb_etats_nfa_tot;
TransitionDFA *liste_transitions;
} Ensemble_etats;

typedef struct Dfa{
Ensemble_etats *debut;
Ensemble_etats **liste_etats;
int nb_etats;
}Dfa;

//on definit les fonctions importantes de cette conversion, d'abors les deux citees dans le cours qui implementent les principes de la conversion puis la fonction de conversion et celle de representation
int* eps_cloture(int* ensemble_depart, int nb_etats_nfa_tot, Nfa *automate);
int* transition(int *ensemble_depart, char lettre, int nb_etats_nfa_tot, Nfa *automate);
Dfa nfa_to_dfa(Nfa *automate);
void graphe_dfa(Dfa *automate);

#endif