#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "logging.h"
#include "tokenList.h"
#include "tokens.h"

typedef struct {
    TokenList* tokens;
    int start;
    int current;
    int line;
    int source_length;
} Scanner;

void scanTokens(TokenList* tokens, char *source, bool *had_error);
bool isAtEnd(Scanner scanner);
void scanToken(Scanner *scanner, char *source, bool *had_error);
char advance(Scanner *scanner, char *source);
void addToken(Scanner *scanner, char *text, TokenType type);
bool match(Scanner *scanner, char *source, char expected);
char peek(Scanner scanner, char *source);
char peekNext(Scanner scanner, char *source);
void addString(Scanner *scanner, char *source, bool *had_error);
bool isDigit(char c);
void addNumber(Scanner *scanner, char *source);
bool isAlpha(char c);
bool isAlphaNumeric(char c);
void identifier(Scanner *scanner, char *source);

#endif // !SCANNER_H

// Clangd hack
#ifndef SCANNER_IMPLEMENTATION_MAIN
#define SCANNER_IMPLEMENTATION
#endif // !SCANNER_IMPLEMENTATION_MAIN

#ifdef SCANNER_IMPLEMENTATION
#undef SCANNER_IMPLEMENTATION

void scanTokens(TokenList* tokens, char *source, bool *had_error) {
    Scanner* scanner = malloc(sizeof(Scanner));
    scanner->tokens = tokens;
    scanner->start = 0;
    scanner->current = 0;
    scanner->line = 1;
    scanner->source_length = strlen(source);

    while (!isAtEnd(*scanner)) {
        scanner->start = scanner->current;
        scanToken(scanner, source, had_error);
    }

    Token* endOfFile = malloc(sizeof(Token));
    endOfFile->type = EOF_I;
    endOfFile->lexeme = NULL;
    endOfFile->literal = NULL;
    endOfFile->line = scanner->line;

    listPush(scanner->tokens, endOfFile);
    free(scanner);
}

bool isAtEnd(Scanner scanner) {
    return scanner.current >= scanner.source_length;
}

void scanToken(Scanner *scanner, char *source, bool *had_error) {
    char c = advance(scanner, source);
    switch (c) {
    // Pure singe Tokens
    case '(':
        addToken(scanner, NULL, LEFT_PAREN);
        break;
    case ')':
        addToken(scanner, NULL, RIGHT_PAREN);
        break;
    case '{':
        addToken(scanner, NULL, LEFT_BRACE);
        break;
    case '}':
        addToken(scanner, NULL, RIGHT_BRACE);
        break;
    case ',':
        addToken(scanner, NULL, COMMA);
        break;
    case '.':
        addToken(scanner, NULL, DOT);
        break;
    case '-':
        addToken(scanner, NULL, MINUS);
        break;
    case '+':
        addToken(scanner, NULL, PLUS);
        break;
    case ';':
        addToken(scanner, NULL, SEMICOLON);
        break;
    case '*':
        addToken(scanner, NULL, STAR);
        break;

    // Single or double Tokens
    case '!':
        addToken(scanner, NULL,
                 match(scanner, source, '=') ? BANG_EQUAL : BANG);
        break;

    case '=':
        addToken(scanner, NULL,
                 match(scanner, source, '=') ? EQUAL_EQUAL : EQUAL);
        break;

    case '<':
        addToken(scanner, NULL,
                 match(scanner, source, '=') ? LESS_EQUAL : LESS);
        break;

    case '>':
        addToken(scanner, NULL,
                 match(scanner, source, '=') ? GREATER_EQUAL : GREATER);
        break;

    // SLASH or a comment
    case '/':
        if (match(scanner, source, '/')) {
            while (peek(*scanner, source) != '\n' && !isAtEnd(*scanner))
                advance(scanner, source);
        } else {
            addToken(scanner, NULL, SLASH);
        }
        break;

    // Whitespace
    case ' ':
    case '\r':
    case '\t':
        break;

    // New line
    case '\n':
        scanner->line++;
        break;

    // Strings
    case '"':
        addString(scanner, source, had_error);
        break;

    default:
        if (isDigit(c)) {
            addNumber(scanner, source);
        } else if (isAlpha(c)) {
            identifier(scanner, source);
        } else {
            error(scanner->line, "Unexpected character.", had_error);
        }
        break;
    }
}

char advance(Scanner *scanner, char *source) {
    return source[scanner->current++];
}

void addToken(Scanner *scanner, char *text, TokenType type) {
    Token* token = malloc(sizeof(Token));
    token->type = type;
    token->lexeme = text;
    token->literal = NULL;
    token->line = scanner->line;
    listPush(scanner->tokens, token);
}

bool match(Scanner *scanner, char *source, char expected) {
    if (isAtEnd(*scanner))
        return false;
    if (source[scanner->current] != expected)
        return false;

    scanner->current++;
    return true;
}

char peek(Scanner scanner, char *source) {
    if (isAtEnd(scanner))
        return '\0';
    return source[scanner.current];
}

char peekNext(Scanner scanner, char *source) {
    if (scanner.current + 1 >= scanner.source_length)
        return '\0';
    return source[scanner.current + 1];
}

void addString(Scanner *scanner, char *source, bool *had_error) {
    // Go to the end of the '"'
    while (peek(*scanner, source) != '"' && !isAtEnd(*scanner)) {
        if (peek(*scanner, source) == '\n')
            scanner->line++;
        advance(scanner, source);
    }

    if (isAtEnd(*scanner)) {
        error(scanner->line, "Unterminated string", had_error);
    }

    // Consume the last '"'
    advance(scanner, source);

    char *text = malloc(scanner->current - scanner->start + 1);
    memcpy(text, source + scanner->start, scanner->current - scanner->start);
    text[scanner->current - scanner->start] = '\0';

    addToken(scanner, text, STRING);
}

bool isDigit(char c) { return c >= '0' && c <= '9'; }

void addNumber(Scanner *scanner, char *source) {
    while (isDigit(peek(*scanner, source)))
        advance(scanner, source);

    if (peek(*scanner, source) == '.' && isDigit(peekNext(*scanner, source))) {
        advance(scanner, source);
        while (isDigit(peek(*scanner, source)))
            advance(scanner, source);
    }

    char *text = malloc(scanner->current - scanner->start + 1);
    memcpy(text, source + scanner->start, scanner->current - scanner->start);
    text[scanner->current - scanner->start] = '\0';

    addToken(scanner, text, NUMBER);
}

bool isAlpha(char c)
{
    return  (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c == '_');
}

bool isAlphaNumeric(char c)
{
    return isAlpha(c) || isDigit(c);
}

void identifier(Scanner* scanner, char* source)
{
    while (isAlphaNumeric(peek(*scanner, source)))
        advance(scanner, source);

    char* text = malloc(scanner->current - scanner->start + 1);
    memcpy(
        text,
        source + scanner->start, scanner->current - scanner->start
    );
    text[scanner->current - scanner->start] = '\0';
    TokenType type = IDENTIFIER;

    if (strcmp(text, "and") == 0) {
        type = AND;
    } else if (strcmp(text, "class") == 0) {
        type = CLASS;
    } else if (strcmp(text, "else") == 0) {
        type = ELSE;
    } else if (strcmp(text, "false") == 0) {
        type = FALSE;
    } else if (strcmp(text, "for") == 0) {
        type = FOR;
    } else if (strcmp(text, "fun") == 0) {
        type = FUN;
    } else if (strcmp(text, "if") == 0) {
        type = IF;
    } else if (strcmp(text, "nil") == 0) {
        type = NIL;
    } else if (strcmp(text, "or") == 0) {
        type = OR;
    } else if (strcmp(text, "print") == 0) {
        type = PRINT;
    } else if (strcmp(text, "return") == 0) {
        type = RETURN;
    } else if (strcmp(text, "super") == 0) {
        type = SUPER;
    } else if (strcmp(text, "this") == 0) {
        type = THIS;
    } else if (strcmp(text, "true") == 0) {
        type = TRUE;
    } else if (strcmp(text, "var") == 0) {
        type = VAR;
    } else if (strcmp(text, "while") == 0) {
        type = WHILE;
    }
    
    addToken(scanner, NULL, type);
}

#endif // SCANNER_IMPLEMENTATION
