#ifndef AST_H
#define AST_H

typedef enum {
    AST_CHAR,    
    AST_UNION,   
    AST_CONCAT,
    AST_STAR     
} AstType;

typedef struct Ast {
    AstType type;
    char value;           
    struct Ast *gauche;    
    struct Ast *droit;   
} Ast;

#endif