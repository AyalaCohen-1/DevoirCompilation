#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "lecteur_exp.h"
#include "nfa.h"

int main() {
    char *expression = "(a|b)*c";
    printf("1. Lecture de l'expression : %s\n", expression);

    Ast *racine = lire_expression(expression);
    if (racine == NULL) {
        printf("Erreur : l'arbre n'a pas pu etre construit.\n");
        return 1;
    }

    printf("2. Generation de l'AST...\n");
    graphe_ast(racine);

    printf("3. Transformation de l'AST en NFA (Thompson)...\n");
    Nfa mon_automate = ast_to_nfa(racine);
    
    printf("4. Generation du fichier nfa.dot...\n");
    graphe_nfa(&mon_automate);

    printf("Termine ! Verifie tes fichiers .dot.\n");
    return 0;
}