#include "dfa.h"
#include <stdlib.h>
#include <stdio.h>

static Ensemble_etats* creer_ensemble(int numero, int nb_total_nfa) {
    Ensemble_etats *nouvel_ensemble = (Ensemble_etats*) malloc(sizeof(Ensemble_etats));
    if (nouvel_ensemble == NULL) {
        printf("Erreur creation nouvel ensemble\n");
        exit(1);
    }
    nouvel_ensemble->no = numero;
    nouvel_ensemble->nb_etats_nfa_tot = nb_total_nfa;
    nouvel_ensemble->est_final = 0;
    nouvel_ensemble->liste_transitions = NULL;
    nouvel_ensemble->etats_nfa = (int*) calloc(nb_total_nfa, sizeof(int)); //on cree un tableau a la base rempli de 0 et qui va petit a petit se remplir de 1 aux indices des no des etats presents dans l'ensemble.   
    return nouvel_ensemble;
}

//on definit la premiere fonction decrite dans le cours, a l'aide d'un tableau qui cree des ensembles d'etats avec des 1 aux indices des etats inclus dans l'ensemble  
int* eps_cloture(int* ensemble_depart, int nb_etats_nfa_tot, Etat **table_etats) {
    int* resultat= (int*)calloc(nb_etats_nfa_tot,sizeof(int));
    for(int i =0; i<nb_etats_nfa_tot; i++){
        resultat[i]=ensemble_depart[i];
    }
    int changement = 1;
    while (changement == 1) {
        changement = 0;

        for (int i=0; i<nb_etats_nfa_tot;i++){
            if (resultat[i]==1){
                if(table_etats[i]->symbole==EPSILON && table_etats[i]->out1!=NULL){
                    if (resultat[table_etats[i]->out1->no] == 0) { 
                        resultat[table_etats[i]->out1->no] = 1;
                        changement = 1;  
                    }    
                }
                 if(table_etats[i]->symbole==EPSILON && table_etats[i]->out2!=NULL){
                   if (resultat[table_etats[i]->out2->no] == 0) { 
                        resultat[table_etats[i]->out2->no] = 1;
                        changement = 1;
                   }       
                }
                
            }
        }
    }
    return resultat;
}

//on passe a la deuxieme fonction definie dans le cours, 
int* transition(int *ensemble_depart, char lettre, int nb_etats_nfa_tot, Etat **table_etats) {
    
    int* resultat=(int*)calloc(nb_etats_nfa_tot, sizeof(int));

    for (int i=0;i<nb_etats_nfa_tot; i++){
        if (ensemble_depart[i]==1){
            if (table_etats[i]->symbole == lettre&& table_etats[i]->out1!=NULL){//on ne regarde que out1 parce que d'apres l'algo du nfa un etat ne peut avoir que 2 transitions max soit eps et une lettre soit 2 epsilon
                resultat[table_etats[i]->out1->no]=1;
            }
        }

    }
    return resultat;
}


//maintenant la fonction de conversion mais d'abord on defini trois fonctions qui seront necessaires pour son implementation
//l'une sert a comparer deux tableaux (les ensembles d'etats)et verifie si ils sont identiques pour eviter que notre code se poursuive sans arret
static int tableaux_identiques(int *tab1, int *tab2, int taille) {
    for (int i = 0; i < taille; i++) {
        if (tab1[i] != tab2[i]) return 0;
    }
    return 1;
}

// une autre une fonction récursive qui defini comment on va remplir la table d'etats du nfa
static void remplir_table(Etat *courant, Etat **table) {
    if (courant == NULL || table[courant->no] != NULL) return;
    table[courant->no] = courant; // On enregistre l'état dans la case de son numéro
    remplir_table(courant->out1, table);
    remplir_table(courant->out2, table);
}

// et cette fonction va creer la table et la remplir
static Etat** creer_table_etats(Nfa *automate, int nb_etats_tot) {
    Etat **table = (Etat**)calloc(nb_etats_tot, sizeof(Etat*));
    remplir_table(automate->debut, table);
    return table;
}

Dfa nfa_to_dfa(Nfa *automate) {
    Dfa resultat_dfa;
    int nb_etats= get_nb_etats_totals();
    int nb_nfa = nb_etats;
    Etat **table_etats = creer_table_etats(automate, nb_nfa);
    
    // On crée un grand tableau qui va stocker tous les ensembles d'etats du DFA créés.
    int capacite_max = 2000;
    Ensemble_etats **liste_dfa = (Ensemble_etats**) malloc(capacite_max * sizeof(Ensemble_etats*));
    int nb_dfa_crees = 0;   // Combien on en a créé au total
    int index_traite = 0;   // Ou on en est dans notre analyse
    int* depart_nfa= (int*)calloc(nb_etats, sizeof(int));
    depart_nfa[automate->debut->no]=1;
    int* tableau_init=eps_cloture(depart_nfa,nb_nfa, table_etats);
    Ensemble_etats *premier_etat = creer_ensemble(0, nb_nfa);
    liste_dfa[0]=premier_etat;
    nb_dfa_crees++;
    resultat_dfa.debut=liste_dfa[0];

    while (index_traite < nb_dfa_crees) {//on cree ici tous les ensembles d'etats autres que l'etat initial
        Ensemble_etats *etat_courant = liste_dfa[index_traite];
    }
    return resultat_dfa;
}

void graphe_dfa(Dfa *automate){//fonction de representation graphique du dfa 
    if (automate == NULL) return; 
    FILE *f = fopen("dfa.dot", "w");
    fprintf(f, "digraph DFA {\n");
    for (int i = 0; i < automate->nb_etats; i++) {
        Ensemble_etats *etat = automate->liste_etats[i];
        if (etat->est_final == 1) { // Si c'est un état final, on met un double cercle 
            fprintf(f, "  %d [shape=doublecircle];\n", etat->no);
        } else {
            fprintf(f, "  %d [shape=circle];\n", etat->no);
        }
    }
    for (int i = 0; i < automate->nb_etats; i++) {
        Ensemble_etats *etat = automate->liste_etats[i];
        TransitionDFA *trans = etat->liste_transitions;
        while (trans != NULL) {
            fprintf(f, "  %d -> %d [label=\"%c\"];\n", etat->no, trans->destination->no, trans->lettre);
            trans = trans->suivante;
        }
    }
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng dfa.dot -o dfa.png"); //automatise la creation de l'image comme pour ast.h et nfa.h
}