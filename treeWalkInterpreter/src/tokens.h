#ifndef TOKENS_H
#define TOKENS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    // Single character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    EOF_I
} TokenType;

typedef enum {
    STRING_LITERAL,
    NUMBER_LITERAL,
    BOOL_LITERAL,
    NONE_LITERAL
} LiteralType;

union Literal_union {
    double number;
    char *string;
    bool boolean;
};

typedef struct {
    LiteralType type;
    union Literal_union object;
} Literal;

typedef struct {
    TokenType type;
    char *lexeme;
    Literal literal;
    int line;
} Token;

void freeToken(Token *token);
void printToken(Token token);
void printLiteralObj(Literal literal);

#endif // TOKENS_H

#ifdef TOKEN_IMPLENEMTATION
#undef TOKEN_IMPLENEMTATION

void freeToken(Token *token) {
    free(token->lexeme);
    if (token->type == STRING)
        free(token->literal.object.string);
}

void printToken(Token token) {
    char *token_type_string;
    switch (token.type) {
    case LEFT_PAREN:
        token_type_string = "LEFT_PAREN";
        break;
    case RIGHT_PAREN:
        token_type_string = "RIGHT_PAREN";
        break;
    case LEFT_BRACE:
        token_type_string = "LEFT_BRACE";
        break;
    case RIGHT_BRACE:
        token_type_string = "RIGHT_BRACE";
        break;
    case COMMA:
        token_type_string = "COMMA";
        break;
    case DOT:
        token_type_string = "DOT";
        break;
    case MINUS:
        token_type_string = "MINUS";
        break;
    case PLUS:
        token_type_string = "PLUS";
        break;
    case SEMICOLON:
        token_type_string = "SEMICOLON";
        break;
    case SLASH:
        token_type_string = "SLASH";
        break;
    case STAR:
        token_type_string = "STAR";
        break;
    case BANG:
        token_type_string = "BANG";
        break;
    case BANG_EQUAL:
        token_type_string = "BANG_EQUAL";
        break;
    case EQUAL:
        token_type_string = "EQUAL";
        break;
    case EQUAL_EQUAL:
        token_type_string = "EQUAL_EQUAL";
        break;
    case GREATER:
        token_type_string = "GREATER";
        break;
    case GREATER_EQUAL:
        token_type_string = "GREATER_EQUAL";
        break;
    case LESS:
        token_type_string = "LESS";
        break;
    case LESS_EQUAL:
        token_type_string = "LESS_EQUAL";
        break;
    case IDENTIFIER:
        token_type_string = "IDENTIFIER";
        break;
    case STRING:
        token_type_string = "STRING";
        break;
    case NUMBER:
        token_type_string = "NUMBER";
        break;
    case AND:
        token_type_string = "AND";
        break;
    case CLASS:
        token_type_string = "CLASS";
        break;
    case ELSE:
        token_type_string = "ELSE";
        break;
    case FALSE:
        token_type_string = "FALSE";
        break;
    case FUN:
        token_type_string = "FUN";
        break;
    case FOR:
        token_type_string = "FOR";
        break;
    case IF:
        token_type_string = "IF";
        break;
    case NIL:
        token_type_string = "NIL";
        break;
    case OR:
        token_type_string = "OR";
        break;
    case PRINT:
        token_type_string = "PRINT";
        break;
    case RETURN:
        token_type_string = "RETURN";
        break;
    case SUPER:
        token_type_string = "SUPER";
        break;
    case THIS:
        token_type_string = "THIS";
        break;
    case TRUE:
        token_type_string = "TRUE";
        break;
    case VAR:
        token_type_string = "VAR";
        break;
    case WHILE:
        token_type_string = "WHILE";
        break;
    case EOF_I:
        token_type_string = "EOF";
        break;
    }
    if (token.type == STRING)
        printf("(%i) %s -- %s --- %s\n", token.line, token_type_string,
               token.lexeme, token.literal.object.string);
    else if (token.type == NUMBER)
        printf("(%i) %s -- %s --- %lf\n", token.line, token_type_string,
               token.lexeme, token.literal.object.number);
    else
        printf("(%i) %s -- %s\n", token.line, token_type_string, token.lexeme);
}

void printLiteralObj(Literal literal) {
    if (literal.type == STRING_LITERAL)
        printf("String: %s\n", literal.object.string);
    else if (literal.type == NUMBER_LITERAL)
        printf("Number: %lf\n", literal.object.number);
    else if (literal.type == BOOL_LITERAL)
        printf("Boolean: %s\n", literal.object.boolean ? "True" : "False");
    else
        printf("Error: Literal type(s) error\n");
}

#endif // TOKEN_IMPLENEMTATION
