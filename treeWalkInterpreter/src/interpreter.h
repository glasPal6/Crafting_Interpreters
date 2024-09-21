#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "tokens.h"

void visitExpr(Expr *expr);
void visitGrouping(Expr *expr);
void visitBinary(Expr *expr);
Literal visitLiteral(Expr *expr);
void visitUnary(Expr *expr);

#endif // !INTERPRETER_H

#ifdef INTERPRETER_IMPLEMENTATION
#undef INTERPRETER_IMPLEMENTATION

void visitExpr(Expr *expr) {
    switch (expr->type) {
    case EXPR_GROUPING:
        visitGrouping(expr);
        break;
    case EXPR_BINARY:
        visitBinary(expr);
        break;
    case EXPR_LITERAL:
        visitLiteral(expr);
        break;
    case EXPR_UNARY:
        visitUnary(expr);
        break;
    }
}

void visitGrouping(Expr *expr) { visitExpr((Expr *)expr->value.grouping); }

void visitBinary(Expr *expr) {
    visitExpr((Expr *)expr->value.binary.left);
    visitExpr((Expr *)expr->value.binary.right);
}

Literal visitLiteral(Expr *expr) { return expr->value.literal.token.literal; }

void visitUnary(Expr *expr) { visitExpr((Expr *)expr->value.unary.right); }

#endif // !INTERPRETER_IMPLEMENTATION
