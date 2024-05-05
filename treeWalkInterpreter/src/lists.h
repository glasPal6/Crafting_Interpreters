#ifndef _LISTS_H
#define _LISTS_H

#include "tokens.h"
#include <stdlib.h>

typedef struct Node {
    Token token;
    struct Node* next;
    int index;
} Node;

void listPush(Node* list, Token token);
Token listPopEnd(Node* list);

#endif // !_LISTS_H

// Clangd hack
#ifndef LISTS_IMPLEMENTATION_MAIN
#define LISTS_IMPLEMENTATION
#endif // !LISTS_IMPLEMENTATION_MAIN 

#ifdef LISTS_IMPLEMENTATION

void listPush(Node* list, Token token)
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        exit(1);
    }
    node->token = token;
    node->next = NULL;

    if (list == NULL) {
        list = node;
        list->index = 0;
    } else {
        Node* list_node = list;
        int count = 0;
        while (list_node->next != NULL) { 
            list_node = list_node->next;
            count += 1;
        }
        list_node->next = node;
        list_node->index += count;
    }
}

Token listPopEnd(Node* list)
{
    if (list == NULL) {
        exit(1);
    }

    Token token;

    Node* list_node = list;
    while (list_node->next->next != NULL) list_node = list_node->next;
    
    token = list_node->next->token;

    Node* last_node = list_node->next;
    list_node->next = NULL;
    free(last_node);

    return  token;
}

#endif //LISTS_IMPLEMENTATION

