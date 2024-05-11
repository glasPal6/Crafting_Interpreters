#ifndef TOKENLIST_H
#define TOKENLIST_H

// #define TOKEN_IMPLENEMTATION
#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct TokenList {
    Token *token;
    struct TokenList* next;
    int index;
} TokenList;

void listPush(TokenList* list, Token* token);
void listPopEnd(TokenList* list);
void listPrint(TokenList* list);

#endif // !TOKENLIST_H

// Clangd hack
#ifndef TOKENLIST_IMPLEMENTATION_MAIN
#define TOKENLIST_IMPLEMENTATION
#endif // !TOKENLIST_IMPLEMENTATION_MAIN 

#ifdef TOKENLIST_IMPLEMENTATION
#undef TOKENLIST_IMPLEMENTATION

void listPush(TokenList* list, Token* token) {
    TokenList* node = malloc(sizeof(TokenList));
    node->token = token;
    node->next = NULL;

    if (list->token == NULL) {
        printf("is null\n");
        list = node; 
        list->index = 0;
    } else {
        printf("is not null\n");
        TokenList list_node = *list; 
        int count = 0;
        while (list_node.next!= NULL) { 
            list_node = *list_node.next;
            count += 1;
        }
        list_node.next = node;
        list_node.index = count;
    }
}

void listPopEnd(TokenList* list)
{
    if (list->token == NULL) {
        printf("List is empty\n");
        return;
    }

    TokenList* list_node = list;
    while (list_node->next->next != NULL)
            list_node = list_node->next;
    
    TokenList* last_node = list_node->next;
    list_node->next = NULL;
    free(last_node->token);
    free(last_node);
}

void listPrint(TokenList* list)
{
    if (list->token == NULL) {
        printf("List is empty\n");
        return;
    }

    TokenList* list_node = list;
    while (list_node != NULL) {
        printToken(*list_node->token);
        list_node = list_node->next;
    }
    free(list_node);
}

#endif //TOKENLIST_IMPLEMENTATION

