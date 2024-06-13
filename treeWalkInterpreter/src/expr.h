#ifndef EXPR_H
#define EXPR_H 

#include "tokens.h"

// Generate this with code
typedef enum {
    EXPR_LITERAL,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_GROUPING
} ExprType;

typedef struct {
    ExprType type;
    union {
        struct {
            Token* token;
            union {
                double number;
                char* string;
            };
        } literal;
        struct {
            Token* token;
            struct Expr* right;
        } unary;
        struct {
            Token* token;
            struct Expr* left;
            struct Expr* right;
        } binary;
        struct Expr* grouping;
    } value;
} Expr;

#endif  // EXPR_H

// Clangd hack
#ifndef EXPR_IMPLEMENTATION_MAIN
#define EXPR_IMPLEMENTATION
#endif // !EXPR_IMPLEMENTATION_MAIN

#ifdef EXPR_IMPLEMENTATION
#undef EXPR_IMPLEMENTATION


#endif // EXPR_IMPLEMENTATION



