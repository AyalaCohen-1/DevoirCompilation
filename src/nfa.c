#include "nfa.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

static int compteur_etats = 0;

Etat* creer_etat(int symbole, Etat *out1, Etat *out2) {
    Etat *nouvel_etat = (Etat*)malloc(sizeof(Etat));
    if (nouvel_etat == NULL) {
        printf("Erreur avec la creation d'un etat\n");
        exit(1); 
    }
    nouvel_etat->symbole = symbole;
    nouvel_etat->out1 = out1;
    nouvel_etat->out2 = out2;
    nouvel_etat->no = compteur_etats;
    compteur_etats++;
    return nouvel_etat;
}

Nfa creer_nfa_char(char c){
    Etat *etat_fin = creer_etat(EPSILON, NULL, NULL);
    Etat *etat_debut = creer_etat(c, etat_fin, NULL);
    Nfa bloc= {etat_debut, etat_fin};
    return bloc;
}

Nfa concat_nfa(Nfa *gauche, Nfa *droit) {
    gauche->fin->symbole = EPSILON;
    gauche->fin->out1 = droit->debut;
    Nfa resultat;
    resultat.debut = gauche-> debut;
    resultat.fin = droit->fin;

    return resultat;
}

Nfa union_nfa(Nfa *a1, Nfa *a2){
    Etat *etat_debut = creer_etat(EPSILON, a1->debut, a2->debut);
    Etat *etat_fin = creer_etat(EPSILON, NULL, NULL);
    a1->fin->symbole = EPSILON;
    a1->fin->out1 = etat_fin;
    a2->fin->symbole = EPSILON;
    a2->fin->out1 = etat_fin;
    Nfa resultat = {etat_debut, etat_fin};
    return resultat;
}

Nfa etoile_nfa(Nfa *a){
    Etat *etat_fin = creer_etat(EPSILON, NULL, NULL);   
    Etat *etat_debut = creer_etat(EPSILON, a->debut, etat_fin);
    a->fin->symbole = EPSILON;
    a->fin->out1 = a->debut;
    a->fin->out2 = etat_fin;

    Nfa resultat = {etat_debut, etat_fin};
    return resultat;
}

Nfa Ast_to_nfa(Ast *arbre) {
    if (arbre ==NULL){
        Nfa automate_nul= {NULL,NULL};
        return automate_nul;
    }
    

}