#ifndef TOKENLIST_H
#define TOKENLIST_H

// #define TOKEN_IMPLENEMTATION
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct TokenList {
    Token token;
    struct TokenList* next;
    int index;
} TokenList;

void listPush(TokenList** list, Token token);
void listPopEnd(TokenList** list);
void listPrint(TokenList* list);

#endif // !TOKENLIST_H

// Clangd hack
#ifndef TOKENLIST_IMPLEMENTATION_MAIN
#define TOKENLIST_IMPLEMENTATION
#endif // !TOKENLIST_IMPLEMENTATION_MAIN 

#ifdef TOKENLIST_IMPLEMENTATION
#undef TOKENLIST_IMPLEMENTATION

void listPush(TokenList** list, Token token)
{
    if (*list != NULL)
        (*list)->index++;

    TokenList* node = (TokenList*)malloc(sizeof(TokenList));
    node->token = token;
    node->next = (*list);
    node->index = 0;
    
    (*list) = node;
}

void listPop(TokenList** list)
{
    if (*list == NULL) {
        printf("List is empty\n");
        return;
    }
    TokenList* list_node = *list;
    *list = (*list)->next;
    free(list_node);
}

void listPrint(TokenList* list)
{
    while (list != NULL) {
        printToken(list->token);
        list = list->next;
    }
}

#endif //TOKENLIST_IMPLEMENTATION

