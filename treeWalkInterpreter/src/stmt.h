#ifndef STMT_H
#define STMT_H

#include <stdio.h>

#include "expr.h"

typedef enum { STMT_EXPR, STMT_PRINT } StmtType;

typedef struct Stmt {
    StmtType type;
    union {
        struct {
            Expr expr;
        } expr;
        struct {
            Expr expr;
        } print;
    } value;
} Stmt;

void printStmt(Stmt stmt);

#endif  // !STMT_H

#ifdef STMT_IMPLEMENTATION
#undef STMT_IMPLEMENTATION

void printStmt(Stmt stmt) {
    switch (stmt.type) {
        case STMT_EXPR:
            printf("Stmt Expr: ");
            printExpr(&stmt.value.expr.expr, 0);
            break;
        case STMT_PRINT:
            printf("Stmt Print: ");
            printExpr(&stmt.value.print.expr, 0);
            break;
    }
}

#endif  // !STMT_IMPLEMENTATION
