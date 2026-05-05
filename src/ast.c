#include <stdlib.h>
#include <stdio.h>
# include "ast.h"

Ast* creer_noeud_char(char c){
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_CHAR;
    noeud->value = c;
    noeud->gauche = NULL;
    noeud->droit = NULL;
    return noeud;
}

Ast* creer_noeud_union(Ast *gauche, Ast *droit){
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_UNION;
    noeud->value= '|' ;
    noeud->gauche = gauche;
    noeud->droit = droit;
    return noeud;
}

Ast* creer_noeud_etoile(Ast *fils){
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_ETOILE;
    noeud->value= '*' ;
    noeud->gauche = fils;
    noeud->droit = NULL;
    return noeud;
}

Ast* creer_noeud_concat(Ast *gauche, Ast *droit){
    Ast* noeud = (Ast*)malloc(sizeof(Ast));
    noeud->type = AST_CONCAT;
    noeud->value= '.' ;
    noeud->gauche = gauche;
    noeud->droit = droit;
    return noeud;
}
void ecrire_noeuds(Ast *n, FILE *f) {
    if (n == NULL) return; 

    fprintf(f, "  \"%p\" [label=\"%c\"];\n", (void*)n, n->value);

    if (n->gauche != NULL) {
        fprintf(f, "  \"%p\" -> \"%p\";\n", (void*)n, (void*)n->gauche);
        ecrire_noeuds(n->gauche, f);
    }

    if (n->droit != NULL) {
        fprintf(f, "  \"%p\" -> \"%p\";\n", (void*)n, (void*)n->droit);
        ecrire_noeuds(n->droit, f);
    }
}

void graphe_ast(Ast *arbre){
    if (arbre == NULL) return; 
    
    FILE *f = fopen("ast.dot", "w");
    fprintf(f, "digraph AST {\n");
    ecrire_noeuds(arbre, f);
    fprintf(f, "}\n");
    fclose(f);
}