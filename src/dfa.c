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
    
    // ÉTAPE 1 : INITIALISATION
    Dfa resultat_dfa;
    int nb_nfa = automate->nb_etats;
    
    // 1a. Crée l'annuaire grâce à l'outil préparatoire
    Etat **table_etats = creer_table_etats(automate, nb_nfa);
    
    // 1b. Crée un grand tableau qui va stocker tous les Ensemble_etats du DFA créés.
    // (On suppose qu'on n'aura pas plus de 1000 états dans le DFA final)
    Ensemble_etats* liste_dfa[1000];
    int nb_dfa_crees = 0;   // Combien on en a créé au total
    int index_traite = 0;   // Où on en est dans notre analyse

    // ÉTAPE 2 : LE TOUT PREMIER ÉTAT (L'état initial du DFA)
    // 2a. Crée un tableau d'entiers 'depart_nfa' (rempli de 0)
    // 2b. Mets un '1' dans la case correspondant au numéro de l'état initial du NFA (automate->debut->no)
    // 2c. Appelle 'eps_cloture' sur ce 'depart_nfa'. Ça te donne le tableau de ton premier état DFA !
    // 2d. Utilise ton usine 'creer_ensemble' pour fabriquer la boîte du premier état.
    // 2e. Range cette boîte dans 'liste_dfa[0]' et augmente 'nb_dfa_crees' de 1.
    // 2f. N'oublie pas de dire que ce premier état est le début de ton DFA : resultat_dfa.debut = liste_dfa[0];

    // ÉTAPE 3 : LA BOUCLE D'ANALYSE (La construction des sous-ensembles)
    char alphabet[2] = {'a', 'b'}; // Les lettres possibles

    while (index_traite < nb_dfa_crees) {
        
        // On prend l'état qu'on doit analyser
        Ensemble_etats *etat_courant = liste_dfa[index_traite];

        // On va tester chaque lettre de l'alphabet (boucle for)
        for (int l = 0; l < 2; l++) {
            char lettre = alphabet[l];

            // 3a. LE SAUT : Appelle 'transition' avec 'etat_courant->etats_nfa' et la 'lettre'
            
            // 3b. LA DIFFUSION : Appelle 'eps_cloture' sur le résultat du saut. 
            // (Ceci est le tableau 'cible_finale' de destination)

            // 3c. VÉRIFICATION DE SÉCURITÉ : Est-ce que 'cible_finale' est totalement vide (que des 0) ?
            // (Si oui, l'état NFA crashait en lisant cette lettre. On ignore et on passe à la lettre suivante, avec 'continue;')

            // 3d. GESTION DES DOUBLONS : Est-ce qu'un état avec ce tableau 'cible_finale' existe DÉJÀ dans 'liste_dfa' ?
            // Fais une boucle sur tous les états déjà créés (de 0 à nb_dfa_crees). 
            // Utilise 'tableaux_identiques' pour comparer.
            // Si OUI : On récupère juste le pointeur de cet ancien état.
            // Si NON : On crée un NOUVEL Ensemble_etats, on lui donne 'cible_finale', on le met dans 'liste_dfa', et on augmente 'nb_dfa_crees'.

            // 3e. LA FLÈCHE (TransitionDFA) : Maintenant qu'on a le départ et l'arrivée, 
            // crée une nouvelle structure 'TransitionDFA' (avec malloc), donne-lui la 'lettre' et l'état de destination,
            // et attache-la à 'etat_courant->liste_transitions'.
        }
        
        index_traite++; // On a fini avec cet état, on passe au suivant de la liste !
    }

    // ÉTAPE 4 : FINALISATION DU DFA
    // 4a. Mets 'nb_dfa_crees' dans 'resultat_dfa.nb_etats'.
    // 4b. Copie ta 'liste_dfa' dans 'resultat_dfa.liste_etats' (il faudra un malloc pour ce tableau de pointeurs).
    // 4c. Parcours tous tes états DFA créés. Si la case numéro 'automate->fin->no' est à 1 dans leur tableau, alors ce sont des états finaux (est_final = 1).

    return resultat_dfa;
}