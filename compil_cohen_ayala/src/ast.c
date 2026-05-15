#include <stdlib.h>
#include <stdio.h>
# include "ast.h"

Ast* creer_noeud_char(char c){ //cree le noeud "de base", celui des caracteres, qui sera toujours celui le plus bas de l'arbre
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_CHAR;
    noeud->value = c;
    noeud->gauche = NULL;
    noeud->droit = NULL;
    return noeud;
}

Ast* creer_noeud_union(Ast *gauche, Ast *droit){ //cree le noeud de l'union avec ses deux fils 
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_UNION;
    noeud->value= '|' ;
    noeud->gauche = gauche;
    noeud->droit = droit;
    return noeud;
}

Ast* creer_noeud_etoile(Ast *fils){ //cree le noeud de l'etoile avec son fils
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_ETOILE;
    noeud->value= '*' ;
    noeud->gauche = fils;
    noeud->droit = NULL;
    return noeud;
}

Ast* creer_noeud_concat(Ast *gauche, Ast *droit){ //cree le noeud de la concatenation avec ses deux fils(generalement deux noeuds de caracteres)
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_CONCAT;
    noeud->value= '.' ;
    noeud->gauche = gauche;
    noeud->droit = droit;
    return noeud;
}

void ecrire_arbre(Ast *n, FILE *f) { // on cree l'arbre de maniere recursive, on ecrit chaque noeud et ses interactions pour permettre de creer le graphe
    if (n == NULL) return; 

    fprintf(f, "  \"%p\" [label=\"%c\"];\n", (void*)n, n->value);
    
    if (n->gauche != NULL) {
        fprintf(f, "  \"%p\" -> \"%p\";\n", (void*)n, (void*)n->gauche);
        ecrire_arbre(n->gauche, f);
    }

    if (n->droit != NULL) {
        fprintf(f, "  \"%p\" -> \"%p\";\n", (void*)n, (void*)n->droit);
        ecrire_arbre(n->droit, f);
    }
}

void graphe_ast(Ast *arbre){ // cree le fichier du graphe, grace a la fonction decrite juste avant
    if (arbre == NULL) return; 
    
    FILE *f = fopen("ast.dot", "w");
    fprintf(f, "digraph AST {\n");
    ecrire_arbre(arbre, f);
    fprintf(f, "}\n");
    fclose(f);
    system("dot -Tpng ast.dot -o ast.png");//automatise la creation de l'image du graphe, comme si on avait ecrit dans le terminal : dot -Tpng ast.dot -o ast.png
}