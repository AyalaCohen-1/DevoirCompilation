#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "lecteur_exp.h"
#include "nfa.h"
#include "dfa.h"
#include "minimisation_dfa.h"
#include "generateur.h" 

int main(int argc, char *argv[])
{
    FILE *fichier = fopen(argv[1], "r");
    if (fichier == NULL) { //on verifie que le fichier est bien 
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    char expression[512];
    if (fgets(expression, sizeof(expression), fichier) == NULL) {
        printf("Erreur : le fichier est vide.\n");
        fclose(fichier);
        return 1;
    }
    fclose(fichier);

    // Supprimme le caractère newline (\n) à la fin
    expression[strcspn(expression, "\r\n")] = 0;
    
    Ast *mon_arbre = lire_expression(expression);
    graphe_ast(mon_arbre); // Genere l'ast

    Nfa mon_nfa = ast_to_nfa(mon_arbre);
    graphe_nfa(&mon_nfa); // Genere le nfa

    Dfa mon_dfa = nfa_to_dfa(&mon_nfa);
    graphe_dfa(&mon_dfa); // Genere le dfa

    Dfa mon_dfa_min = dfa_minimal(&mon_dfa);
    graphe_dfa_minimal(&mon_dfa_min);

    generer_code_c(&mon_dfa_min, expression);

    return 0;
}