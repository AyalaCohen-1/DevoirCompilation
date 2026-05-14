#include "nfa.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

static int compteur_etats = 0;

Etat* creer_etat(int symbole, Etat *out1, Etat *out2) { //un NFA fonctionne avec des etats et des transitions, la base c'est de creer les etats
    Etat *nouvel_etat = (Etat*)malloc(sizeof(Etat));
    if (nouvel_etat == NULL) {
        printf("Erreur avec la creation de l'etat\n");
        exit(1); 
    }
    nouvel_etat->symbole = symbole;
    nouvel_etat->out1 = out1;
    nouvel_etat->out2 = out2;
    nouvel_etat->no = compteur_etats;
    compteur_etats++;
    return nouvel_etat;
}

Nfa creer_nfa_char(char c){//la on cree une sorte de "mini NFA" qu'on va relier a d'autres "blocs" pour construire le Nfa final
    Etat *etat_fin = creer_etat(EPSILON, NULL, NULL);
    Etat *etat_debut = creer_etat(c, etat_fin, NULL);
    Nfa bloc= {etat_debut, etat_fin};
    return bloc;
}


//on procede de meme pour les autres types de relations entre les etats en suivant l'algo de McNaughton, Yamada et Thompson
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

Nfa ast_to_nfa(Ast *arbre) {//au lieu de creer le Nfa a partir de l'expression reguliere on "convertit" l'ast.
    if (arbre ==NULL){
        Nfa automate_nul= {NULL,NULL};
        return automate_nul;
    }
    switch(arbre->type){ //on definit la conversion a effectuer par raport au type de noeud
        case AST_CHAR:
            return creer_nfa_char(arbre->value);
        case AST_UNION:{
            Nfa gauche = ast_to_nfa(arbre->gauche);
            Nfa droit = ast_to_nfa(arbre->droit);
            return union_nfa(&gauche, &droit);
        }
        case AST_CONCAT:{
            Nfa gauche = ast_to_nfa(arbre->gauche);
            Nfa droit = ast_to_nfa(arbre->droit);
            return concat_nfa(&gauche, &droit);
        }
        case AST_ETOILE:{
            Nfa fils = ast_to_nfa(arbre->gauche);
            return etoile_nfa(&fils); 
        }
    }
    
}

static void parcours_nfa(FILE *f, Etat *etat, int *visites) { //fonction pour parcourir l'automate de maniere recursive
    if (etat == NULL || visites[etat->no] == 1) {
        return;
    }
    visites[etat->no] = 1;
    if (etat->out1 != NULL) {
        if (etat->symbole == EPSILON) {
            fprintf(f, "  %d -> %d [label=\"eps\"];\n", etat->no, etat->out1->no);
        } else {
            fprintf(f, "  %d -> %d [label=\"%c\"];\n", etat->no, etat->out1->no, etat->symbole);
        }
        parcours_nfa(f, etat->out1, visites);
    }

    if (etat->out2 != NULL) {
        fprintf(f, "  %d -> %d [label=\"eps\"];\n", etat->no, etat->out2->no);
        parcours_nfa(f, etat->out2, visites);
    }
}

void graphe_nfa(Nfa *automate){//meme principe que dans ast.c
    if (automate == NULL) return; 
    FILE *f = fopen("nfa.dot", "w");
    fprintf(f, "digraph NFA {\n");
    fprintf(f, "  %d [shape=doublecircle];\n", automate->fin->no);
    int visites[1000] = {0}; //fixe un nb de visites max pour eviter que ca continue infiniment (on suppose que 1000 ca suffit)
    parcours_nfa(f, automate->debut, visites);
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng nfa.dot -o nfa.png"); //automatise la creation de l'image comme pour ast.h
}

// rajoute plus tard pour dfa.c
int get_nb_etats_total() {
    return compteur_etats; 
}