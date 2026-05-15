#include "dfa.h"
#include <stdlib.h>
#include <stdio.h>
#include "minimisation_dfa.h"
#include "lecteur_exp.h"

// verifie vers quel état on va si on lit cette lettre 
static int va_vers(Ensemble_etats *etat, char lettre) {
    TransitionDFA *t = etat->liste_transitions;
    while (t != NULL) {
        if (t->lettre == lettre) return t->destination->no;
        t = t->suivante;
    }
    return -1; // -1 = Pas de transition
}

// Verifie si deux états ont exactement le même destination
static int meme_direction(int e1, int e2, int *groupe, char *alphabet, int nb_lettres, Dfa *automate) {
    if (groupe[e1] != groupe[e2]) return 0; // S'ils sont déjà dans des groupes differents on n'a meme pas besoin de verifier
    
    for (int i = 0; i < nb_lettres; i++) {
        int d1 = va_vers(automate->liste_etats[e1], alphabet[i]);
        int d2 = va_vers(automate->liste_etats[e2], alphabet[i]);
        
        int groupe_d1 = (d1 == -1) ? -1 : groupe[d1];
        int groupe_d2 = (d2 == -1) ? -1 : groupe[d2];
        
        if (groupe_d1 != groupe_d2) return 0; // pas la meme destination
    }
    return 1; // identiques
}

// fonction pour créer un état vide pour y mettre les groupes d'etats et chacun de ses etats originellement vides sera un etat du dfa min
static Ensemble_etats* creer_etat_min(int no, int taille) {
    Ensemble_etats *e = (Ensemble_etats*) malloc(sizeof(Ensemble_etats));
    e->no = no; e->est_final = 0; e->liste_transitions = NULL;
    e->etats_nfa = (int*) calloc(taille, sizeof(int));
    return e;
}

Dfa dfa_minimal(Dfa *automate) { //fonction qui va creer le dfa min a partir de l'algorithme décrit dans le cours
    if (automate->nb_etats == 0) return *automate;

    extern char alphabet[256];
    extern int taille_alphabet;
    int n = automate->nb_etats;
    
    int *groupe = (int*)malloc(n * sizeof(int));
    int nb_groupes = 2; // Au depart on separe en 2 groupes :le groupe non-finaux et le groupe finaux

    for (int i = 0; i < n; i++) {
        groupe[i] = automate->liste_etats[i]->est_final ? 1 : 0;
    }

    int changement = 1;
    while (changement) {
        changement = 0;
        int next_id = 0;
        int *nouveau_groupe = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) nouveau_groupe[i] = -1;

        for (int i = 0; i < n; i++) {
            if (nouveau_groupe[i] == -1) {
                nouveau_groupe[i] = next_id;
                for (int j = i + 1; j < n; j++) {
                    if (nouveau_groupe[j] == -1 && meme_direction(i, j, groupe, alphabet, taille_alphabet, automate)) {
                        nouveau_groupe[j] = next_id; // On met les etats avec la meme destination et le meme chemin dans le meme groupe
                    }
                }
                next_id++;
            }
        }
        
        if (next_id > nb_groupes) { // Si on a créé de nouveaux groupes, on recommence un tour
            changement = 1;
            nb_groupes = next_id;
            for (int i = 0; i < n; i++) groupe[i] = nouveau_groupe[i];
        }
        free(nouveau_groupe);
    }

    Dfa min;
    min.nb_etats = nb_groupes;
    min.liste_etats = (Ensemble_etats**)malloc(nb_groupes * sizeof(Ensemble_etats*));

    // on cree les etats du dfa min
    for (int k = 0; k < nb_groupes; k++) {
        min.liste_etats[k] = creer_etat_min(k, n);
        for (int i = 0; i < n; i++) {
            if (groupe[i] == k) {
                if (automate->debut->no == i) min.debut = min.liste_etats[k];
                if (automate->liste_etats[i]->est_final) min.liste_etats[k]->est_final = 1;
            }
        }
    }

    for (int k = 0; k < nb_groupes; k++) {
        int rep = -1;
        for (int i = 0; i < n; i++) if (groupe[i] == k) { rep = i; break; } //on choisit un etat qui sera celui qu'on gardera comme referenc pour creer les etats du dfa min
        
        for (int l = 0; l < taille_alphabet; l++) {
            int dest = va_vers(automate->liste_etats[rep], alphabet[l]);
            if (dest != -1) {
                TransitionDFA *tr = (TransitionDFA*)malloc(sizeof(TransitionDFA));
                tr->lettre = alphabet[l];
                tr->destination = min.liste_etats[groupe[dest]];
                tr->suivante = min.liste_etats[k]->liste_transitions;
                min.liste_etats[k]->liste_transitions = tr;
            }
        }
    }
    
    free(groupe);
    return min;
}    


void graphe_dfa_minimal(Dfa *automate){//fonction de representation graphique du dfa minimal, c'est la meme que pour le dfa "normal" car le dfa minimal est toujours un dfa
    if (automate == NULL) return; 
    FILE *f = fopen("dfa_min.dot", "w");
    fprintf(f, "digraph DFA_MIN {\n");
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
    system("dot -Tpng dfa_min.dot -o dfa_min.png"); //automatise la creation de l'image comme pour ast.h et nfa.h et dfa.h
}