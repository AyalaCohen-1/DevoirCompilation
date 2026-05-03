#ifndef NODE_H
#define NODE_H

typedef enum {
    NODE_CHAR,    
    NODE_UNION,   
    NODE_CONCAT,
    NODE_STAR     
} NodeType;

typedef struct Node {
    NodeType type;
    char value;           
    struct Node *left;    
    struct Node *right;   
} Node;

#endif