#ifndef _TOKENS_H
#define _TOKENS_H

#include <stdio.h>

typedef enum {
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    EOF_I
} TokenType ;

typedef struct {
    TokenType type;
    char* lexeme;
    char* literal; // This may cause problems later
    int line;
} Token;

void printToken(Token *token);

#endif // _TOKENS_H

// Clangd hack
#ifndef TOKEN_IMPLENEMTATION_MAIN
#define TOKEN_IMPLENEMTATION
#endif // !TOKEN_IMPLENEMTATION_MAIN

#ifdef TOKEN_IMPLENEMTATION

void prinprintToken(Token *token)
{
    char* token_type_string;
    switch (token->type) {
        case LEFT_PAREN: token_type_string = "LEFT_PAREN"; break;
        case RIGHT_PAREN: token_type_string = "RIGHT_PAREN"; break;
        case LEFT_BRACE: token_type_string = "LEFT_BRACE"; break;
        case RIGHT_BRACE: token_type_string = "RIGHT_BRACE"; break;
        case COMMA: token_type_string = "COMMA"; break;
        case DOT: token_type_string = "DOT"; break;
        case MINUS: token_type_string = "MINUS"; break;
        case PLUS: token_type_string = "PLUS"; break;
        case SEMICOLON: token_type_string = "SEMICOLON"; break;
        case SLASH: token_type_string = "SLASH"; break;
        case STAR: token_type_string = "STAR"; break;
        case BANG: token_type_string = "BANG"; break;
        case BANG_EQUAL: token_type_string = "BANG_EQUAL"; break;
        case EQUAL: token_type_string = "EQUAL"; break;
        case EQUAL_EQUAL: token_type_string = "EQUAL_EQUAL"; break;
        case GREATER: token_type_string = "GREATER"; break;
        case GREATER_EQUAL: token_type_string = "GREATER_EQUAL"; break;
        case LESS: token_type_string = "LESS"; break;
        case LESS_EQUAL: token_type_string = "LESS_EQUAL"; break;
        case IDENTIFIER: token_type_string = "IDENTIFIER"; break;
        case STRING: token_type_string = "STRING"; break;
        case NUMBER: token_type_string = "NUMBER"; break;
        case AND: token_type_string = "AND"; break;
        case CLASS: token_type_string = "CLASS"; break;
        case ELSE: token_type_string = "ELSE"; break;
        case FALSE: token_type_string = "FALSE"; break;
        case FUN: token_type_string = "FUN"; break;
        case FOR: token_type_string = "FOR"; break;
        case IF: token_type_string = "IF"; break;
        case NIL: token_type_string = "NIL"; break;
        case OR: token_type_string = "OR"; break;
        case PRINT: token_type_string = "PRINT"; break;
        case RETURN: token_type_string = "RETURN"; break;
        case SUPER: token_type_string = "SUPER"; break;
        case THIS: token_type_string = "THIS"; break;
        case TRUE: token_type_string = "TRUE"; break;
        case VAR: token_type_string = "VAR"; break;
        case WHILE: token_type_string = "WHILE"; break;
        case EOF_I: token_type_string = "EOF"; break;
    }
    printf("%s - %s - %s", token_type_string, token->lexeme, token->literal);
}

#endif // TOKEN_IMPLENEMTATION 
