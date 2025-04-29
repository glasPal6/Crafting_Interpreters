#ifndef STMTLIST_H
#define STMTLIST_H

#include <stdint.h>

#include "expr.h"
#include "stmt.h"

typedef struct StmtList {
    Stmt stmt;
    struct StmtList *next;
    uint32_t index;
} StmtList;

void stmtListPushEnd(StmtList **list, Stmt stmt);
void stmtListPushStart(StmtList **list, Stmt stmt);
void stmtListPop(StmtList **list);

Stmt stmtListIndexOf(StmtList **list, uint32_t index);

void stmtListPrint(StmtList *list);

#endif // !STMTLIST_H

#ifdef STMTLIST_IMPLEMENTATION
#undef STMTLIST_IMPLEMENTATION

void StmtListPushEnd(StmtList **list, Stmt Stmt) {
    StmtList *node = (StmtList *)malloc(sizeof(StmtList));
    node->stmt = Stmt;
    node->next = NULL;
    node->index = 0;

    if (*list == NULL) {
        *list = node;
        return;
    } else {
        StmtList *last = *list;
        while (last->next != NULL)
            last = last->next;
        last->next = node;
        node->index = last->index + 1;
    }
}

void StmtListPushStart(StmtList **list, Stmt Stmt) {
    if (*list != NULL)
        (*list)->index++;

    StmtList *node = (StmtList *)malloc(sizeof(StmtList));
    node->stmt = Stmt;
    node->next = (*list);
    node->index = 0;

    (*list) = node;
}

void StmtListPop(StmtList **list) {
    if (*list == NULL) {
        printf("List is empty\n");
        return;
    }
    StmtList *list_node = *list;
    *list = (*list)->next;
    switch (list_node->stmt.type) {
    case STMT_EXPR:
        clearExpr(&list_node->stmt.value.expr.expr);
    case STMT_PRINT:
        clearExpr(&list_node->stmt.value.print.expr);
    }
    free(list_node);
}

Stmt StmtListIndexOf(StmtList **list, uint32_t index) {
    StmtList *node = *list;
    while (node != NULL) {
        if (node->index == index)
            return node->stmt;
        node = node->next;
    }
    Token null_literal = {.type = EOF_I,
                          .lexeme = "",
                          .literal.object.string = "",
                          .literal.type = NONE_LITERAL,
                          .line = -1};
    Expr null_expr = {
        .type = EXPR_LITERAL, .line = -1, .value.literal.token = null_literal};
    Stmt null_Stmt = {.type = STMT_EXPR, .value.expr = {null_expr}};
    return null_Stmt;
}

void StmtListPrint(StmtList *list) {
    if (list == NULL) {
        printf("List has no Stmts\n");
        return;
    }
    while (list != NULL) {
        printf("[%d] ", list->index);
        printStmt(list->stmt);
        list = list->next;
    }
}

#endif // STMTLIST_IMPLEMENTATION
