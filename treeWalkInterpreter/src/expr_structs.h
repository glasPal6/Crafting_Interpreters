#ifndef EXPR_H
#define EXPR_H

#include "tokens.h"

// Generated this with code
typedef enum { EXPR_GROUPING, EXPR_BINARY, EXPR_LITERAL, EXPR_UNARY } ExprType;

typedef struct {
    ExprType type;
    union {
        struct Expr *grouping;
        struct {
            struct Expr *left;
            Token *token;
            struct Expr *right;
        } binary;
        struct {
            Token *token;
            union Literal *literal;
        } literal;
        struct {
            Token *token;
            struct Expr *right;
        } unary;
    } value;
} Expr;

#endif // EXPR_H
