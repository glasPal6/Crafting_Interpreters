#ifndef EXPR_H
#define EXPR_H

#include "tokens.h"

// Generated this with code
typedef enum { EXPR_GROUPING, EXPR_BINARY, EXPR_LITERAL, EXPR_UNARY } ExprType;

typedef struct Expr {
    ExprType type;
    union {
        struct Expr *grouping;
        struct {
            struct Expr *left;
            Token token;
            struct Expr *right;
        } binary;
        struct {
            Token token;
        } literal;
        struct {
            Token token;
            struct Expr *right;
        } unary;
    } value;
} Expr;

void clearExpr(Expr *expr);

void printExpr(Expr *expr, uint level);
void printGrouping(Expr *expr, uint level);
void printBinary(Expr *expr, uint level);
void printLiteral(Expr *expr, uint level);
void printUnary(Expr *expr, uint level);

#endif // EXPR_H

#ifdef EXPR_IMPLEMENTATION
#undef EXPR_IMPLEMENTATION

void clearExpr(Expr *expr) {
    switch (expr->type) {
    case EXPR_GROUPING:
        clearExpr(expr->value.grouping);
        free(expr);
        break;
    case EXPR_BINARY:
        clearExpr(expr->value.binary.left);
        clearExpr(expr->value.binary.right);
        free(expr);
        break;
    case EXPR_LITERAL:
        free(expr);
        break;
    case EXPR_UNARY:
        clearExpr(expr->value.unary.right);
        free(expr);
        break;
    }
}

void printExpr(Expr *expr, uint level) {
    for (uint i = 0; i < level; i++) {
        printf("| ");
    }
    switch (expr->type) {
    case EXPR_GROUPING:
        printGrouping(expr, level);
        break;
    case EXPR_BINARY:
        printBinary(expr, level);
        break;
    case EXPR_LITERAL:
        printLiteral(expr, level);
        break;
    case EXPR_UNARY:
        printUnary(expr, level);
        break;
    }
}

void printGrouping(Expr *expr, uint level) {
    printf("(%i) Grouping: \n", level);
    printExpr((Expr *)expr->value.grouping, level + 1);
}

void printBinary(Expr *expr, uint level) {
    printf("(%i) Binary: ", level);
    printToken(expr->value.binary.token);
    printExpr((Expr *)expr->value.binary.left, level + 1);
    printExpr((Expr *)expr->value.binary.right, level + 1);
}

void printLiteral(Expr *expr, uint level) {
    printf("(%i) Literal: ", level);
    printToken(expr->value.literal.token);
}

void printUnary(Expr *expr, uint level) {
    printf("(%i) Unary: ", level);
    printToken(expr->value.unary.token);
    printExpr((Expr *)expr->value.unary.right, level + 1);
}

#endif // EXPR_IMPLEMENTATION
