#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int main() {
    printf("Debut du test de l'AST...\n");

    // 1. On crée les feuilles (les caractères 'a' et 'b')
    Ast *noeud_a = creer_noeud_char('a');
    Ast *noeud_b = creer_noeud_char('b');

    // 2. On crée le nœud Union (a|b) qui a pour fils gauche 'a' et fils droit 'b'
    Ast *noeud_union = creer_noeud_union(noeud_a, noeud_b);

    // 3. On crée le nœud Etoile (a|b)* qui a pour fils unique (gauche) le nœud union
    Ast *racine = creer_noeud_etoile(noeud_union);

    // 4. On génère le fichier .dot
    printf("Generation du fichier ast.dot...\n");
    graphe_ast(racine);

    printf("Termine ! Regarde dans ton explorateur de fichiers.\n");

    // (Optionnel pour l'instant si tu n'as pas encore fait la fonction liberer_ast, 
    // mais c'est bien de le mettre plus tard pour faire propre)
    // liberer_ast(racine);

    return 0;
}