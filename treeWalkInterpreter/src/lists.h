#ifndef _LISTS_H
#define _LISTS_H

#include "tokens.h"

typedef struct {
    Token token;
    struct Node* next;
} Node;

typedef struct {
    Node* head;
    Node* tail;
    int count;
} List;

void list_init(List* list);
void list_push(List* list, Token token);
void list_pop_end(List* list);

#endif // !_LISTS_H

// Clangd hack
#ifndef LISTS_IMPLEMENTATION_MAIN
#define LISTS_IMPLEMENTATION
#endif // !LISTS_IMPLEMENTATION_MAIN 


#ifdef LISTS_IMPLEMENTATION

void list_init(List* list)
{
}
void list_push(List* list, Token token)
{
}
void list_pop_end(List* list)
{
    // Be sure to free the node
}

#endif //LISTS_IMPLEMENTATION

