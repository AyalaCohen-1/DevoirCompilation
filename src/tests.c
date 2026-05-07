#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "lecteur_exp.h"

int main() {
    // 1. L'expression à tester
    char *expression = "(a|b)*c";
    printf("Test de lecture de l'expression : %s\n", expression);

    // 2. On lance notre super lecteur !
    Ast *racine = lire_expression(expression);

    if (racine == NULL) {
        printf("Erreur : l'arbre n'a pas pu etre construit.\n");
        return 1;
    }

    // 3. On génère le fichier Graphviz
    printf("Generation du fichier ast.dot...\n");
    graphe_ast(racine);

    printf("Termine ! Regarde ton fichier ast.dot.\n");

    return 0;
}