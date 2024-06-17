#ifndef TOKENLIST_H
#define TOKENLIST_H

#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TokenList {
    Token token;
    struct TokenList *next;
    int index;
} TokenList;

void listPushEnd(TokenList **list, Token token);
void listPushStart(TokenList **list, Token token);
void listPop(TokenList **list);
void listPrint(TokenList *list);

#endif // !TOKENLIST_H

#ifdef TOKENLIST_IMPLEMENTATION
#undef TOKENLIST_IMPLEMENTATION

void listPushEnd(TokenList **list, Token token) {
    TokenList *node = (TokenList *)malloc(sizeof(TokenList));
    node->token = token;
    node->next = NULL;
    node->index = 0;

    if (*list == NULL) {
        *list = node;
        return;
    } else {
        TokenList *last = *list;
        while (last->next != NULL) {
            last = last->next;
            node->index++;
        }
        last->next = node;
    }
}

void listPushStart(TokenList **list, Token token) {
    if (*list != NULL)
        (*list)->index++;

    TokenList *node = (TokenList *)malloc(sizeof(TokenList));
    node->token = token;
    node->next = (*list);
    node->index = 0;

    (*list) = node;
}

void listPop(TokenList **list) {
    if (*list == NULL) {
        printf("List is empty\n");
        return;
    }
    TokenList *list_node = *list;
    *list = (*list)->next;
    freeToken(&list_node->token);
    free(list_node);
}

void listPrint(TokenList *list) {
    if (list == NULL) {
        printf("List has no Tokens\n");
        return;
    }
    while (list != NULL) {
        printToken(list->token);
        list = list->next;
    }
}

#endif // TOKENLIST_IMPLEMENTATION
