#include "lecteur_exp.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

static int position=0;
static char* expression;

static Ast* analyse_char();
static Ast* analyse_etoile();
static Ast* analyse_concat();
static Ast* analyse_union();

Ast* lire_expression(char* exp){
    expression = exp;
    position = 0;
    return analyse_union();
}

static Ast* analyse_char(){
    char c = expression[position];

    if (c == '\0') {
        return NULL;
}
if (c == '(') {
        position++; 
        Ast* noeud_interieur = analyse_union(); 
        
        if (expression[position] == ')') {
            position++; // On saute la ')'
        } else {
            printf("Erreur: parenthèse fermante manquante !\n");
        }
        return noeud_interieur;
}

position++;
return creer_noeud_char(c);
}

static Ast* analyse_etoile() {

    Ast* fils = analyse_char();

    if (fils == NULL) {
        return NULL;
    }

    char c = expression[position];

    if (c == '*') {
        position++;
        return creer_noeud_etoile(fils);
    }

    return fils;
}

static Ast* analyse_concat() {
    Ast* gauche = analyse_etoile();

    if (gauche == NULL) {
        return NULL;
    }
    while (1) {
        char c = expression[position];
        
        if (c == '\0' || c == ')' || c == '*' || c == '|') {
            break;
        }
        Ast* droit = analyse_etoile();
        gauche = creer_noeud_concat(gauche, droit);
    }

    return gauche;
}

static Ast* analyse_union() {
    Ast* gauche = analyse_concat();

    if (gauche == NULL) {
        return NULL;
    }
    
    while (1) {
        char c = expression[position];

        
        if (c == '|') {
            position++;
            Ast* droit = analyse_concat();
            gauche=creer_noeud_union(gauche, droit); 
        } else {
            break; 
        }
    }
    return gauche;
}