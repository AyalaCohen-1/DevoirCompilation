#ifndef AST_H
#define AST_H
//les fichiers .h servent a annoncer en quelque sorte la forme que va prendre le code reel et les fonctions qui seront implementees. 

typedef enum {
    AST_CHAR,    
    AST_UNION,   
    AST_CONCAT,
    AST_ETOILE    
} AstType;

typedef struct Ast {
    AstType type;
    char value;           
    struct Ast *gauche;    
    struct Ast *droit;  
} Ast;

Ast* creer_noeud_char(char c); 
Ast* creer_noeud_union(Ast *gauche, Ast *droit); 
Ast* creer_noeud_concat(Ast *gauche, Ast *droit); 
Ast* creer_noeud_etoile(Ast *fils); 
void graphe_ast(Ast *arbre);

#endif