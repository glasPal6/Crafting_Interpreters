#ifndef SCANNER_H
#define SCANNER_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "logging.h"
#include "tokenList.h"

typedef struct {
    int start;
    int current;
    int line;
    int source_length;
} Scanner;

void scanTokens(TokenList **tokens, char *source, bool *had_error);
bool scannerIsAtEnd(Scanner scanner);
void scanToken(TokenList **tokens, Scanner *scanner, char *source,
               bool *had_error);
char scannerAdvance(Scanner *scanner, char *source);
void scannerAddToken(TokenList **list, Scanner *scanner, char *source,
                     char *literal, TokenType type);
bool scannerMatch(Scanner *scanner, char *source, char expected);
char scannerPeek(Scanner scanner, char *source);
char scannerPeekNext(Scanner scanner, char *source);
void scannerAddString(TokenList **list, Scanner *scanner, char *source,
                      bool *had_error);
bool scannerIsDigit(char c);
void scannerAddNumber(TokenList **list, Scanner *scanner, char *source);
bool scannerIsAlpha(char c);
bool scannerIsAlphaNumeric(char c);
void scannerIdentifier(TokenList **list, Scanner *scanner, char *source);

#endif // !SCANNER_H

#ifdef SCANNER_IMPLEMENTATION
#undef SCANNER_IMPLEMENTATION

void scanTokens(TokenList **tokens, char *source, bool *had_error) {
    Scanner scanner = {
        .current = 0,
        .start = 0,
        .line = 1,
        .source_length = strlen(source),
    };

    while (!scannerIsAtEnd(scanner)) {
        scanner.start = scanner.current;
        scanToken(tokens, &scanner, source, had_error);
    }

    Token endOfFile = {.type = EOF_I,
                       .lexeme = NULL,
                       .literal.object.string = NULL,
                       .literal.type = NONE_LITERAL,
                       .line = scanner.line};

    listPushEnd(tokens, endOfFile);
}

bool scannerIsAtEnd(Scanner scanner) {
    return scanner.current >= scanner.source_length;
}

void scanToken(TokenList **tokens, Scanner *scanner, char *source,
               bool *had_error) {
    char c = scannerAdvance(scanner, source);
    switch (c) {
    // Pure singe Tokens
    case '(':
        scannerAddToken(tokens, scanner, source, NULL, LEFT_PAREN);
        break;
    case ')':
        scannerAddToken(tokens, scanner, source, NULL, RIGHT_PAREN);
        break;
    case '{':
        scannerAddToken(tokens, scanner, source, NULL, LEFT_BRACE);
        break;
    case '}':
        scannerAddToken(tokens, scanner, source, NULL, RIGHT_BRACE);
        break;
    case ',':
        scannerAddToken(tokens, scanner, source, NULL, COMMA);
        break;
    case '.':
        scannerAddToken(tokens, scanner, source, NULL, DOT);
        break;
    case '-':
        scannerAddToken(tokens, scanner, source, NULL, MINUS);
        break;
    case '+':
        scannerAddToken(tokens, scanner, source, NULL, PLUS);
        break;
    case ';':
        scannerAddToken(tokens, scanner, source, NULL, SEMICOLON);
        break;
    case '*':
        scannerAddToken(tokens, scanner, source, NULL, STAR);
        break;

    // Single or double Tokens
    case '!':
        scannerAddToken(tokens, scanner, source, NULL,
                        scannerMatch(scanner, source, '=') ? BANG_EQUAL : BANG);
        break;

    case '=':
        scannerAddToken(tokens, scanner, source, NULL,
                        scannerMatch(scanner, source, '=') ? EQUAL_EQUAL
                                                           : EQUAL);
        break;

    case '<':
        scannerAddToken(tokens, scanner, source, NULL,
                        scannerMatch(scanner, source, '=') ? LESS_EQUAL : LESS);
        break;

    case '>':
        scannerAddToken(tokens, scanner, source, NULL,
                        scannerMatch(scanner, source, '=') ? GREATER_EQUAL
                                                           : GREATER);
        break;

    // SLASH or a comment
    case '/':
        if (scannerMatch(scanner, source, '/')) {
            while (scannerPeek(*scanner, source) != '\n' &&
                   !scannerIsAtEnd(*scanner))
                scannerAdvance(scanner, source);
        } else {
            scannerAddToken(tokens, scanner, source, NULL, SLASH);
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
        scannerAddString(tokens, scanner, source, had_error);
        break;

    default:
        if (scannerIsDigit(c)) {
            scannerAddNumber(tokens, scanner, source);
        } else if (scannerIsAlpha(c)) {
            scannerIdentifier(tokens, scanner, source);
        } else {
            error(scanner->line, "Unexpected character.", had_error);
        }
        break;
    }
}

char scannerAdvance(Scanner *scanner, char *source) {
    return source[scanner->current++];
}

void scannerAddToken(TokenList **list, Scanner *scanner, char *source,
                     char *literal, TokenType type) {
    char *text = (char *)malloc(scanner->current - scanner->start + 1);
    memcpy(text, source + scanner->start, scanner->current - scanner->start);
    text[scanner->current - scanner->start] = '\0';

    Token token;
    if (type == STRING) {
        token.type = type;
        token.lexeme = text;
        token.literal.object.string = literal;
        token.literal.type = STRING_LITERAL;
        token.line = scanner->line;
    } else if (type == NUMBER) {
        token.type = type;
        token.lexeme = text;
        token.literal.object.number = atof(literal);
        token.literal.type = NUMBER_LITERAL;
        token.line = scanner->line;
    } else {
        token.type = type;
        token.lexeme = text;
        token.literal.object.string = literal;
        token.literal.type = NONE_LITERAL;
        token.line = scanner->line;
    }
    listPushEnd(list, token);
}

bool scannerMatch(Scanner *scanner, char *source, char expected) {
    if (scannerIsAtEnd(*scanner))
        return false;
    if (source[scanner->current] != expected)
        return false;

    scanner->current++;
    return true;
}

char scannerPeek(Scanner scanner, char *source) {
    if (scannerIsAtEnd(scanner))
        return '\0';
    return source[scanner.current];
}

char scannerPeekNext(Scanner scanner, char *source) {
    if (scanner.current + 1 >= scanner.source_length)
        return '\0';
    return source[scanner.current + 1];
}

void scannerAddString(TokenList **list, Scanner *scanner, char *source,
                      bool *had_error) {
    // Go to the end of the '"'
    while (scannerPeek(*scanner, source) != '"' && !scannerIsAtEnd(*scanner)) {
        if (scannerPeek(*scanner, source) == '\n')
            scanner->line++;
        scannerAdvance(scanner, source);
    }

    if (scannerIsAtEnd(*scanner)) {
        error(scanner->line - 1, "Unterminated string", had_error);
    }

    // Consume the last '"'
    scannerAdvance(scanner, source);

    // Remove the quotes
    char *text = (char *)malloc(scanner->current - scanner->start + 1 - 2);
    memcpy(text, source + scanner->start + 1,
           scanner->current - scanner->start - 2);
    text[scanner->current - scanner->start - 2] = '\0';

    scannerAddToken(list, scanner, source, text, STRING);
}

bool scannerIsDigit(char c) { return c >= '0' && c <= '9'; }

void scannerAddNumber(TokenList **list, Scanner *scanner, char *source) {
    while (scannerIsDigit(scannerPeek(*scanner, source)))
        scannerAdvance(scanner, source);

    if (scannerPeek(*scanner, source) == '.' &&
        scannerIsDigit(scannerPeekNext(*scanner, source))) {
        scannerAdvance(scanner, source);
        while (scannerIsDigit(scannerPeek(*scanner, source)))
            scannerAdvance(scanner, source);
    }

    char *text = (char *)malloc(scanner->current - scanner->start + 1);
    memcpy(text, source + scanner->start, scanner->current - scanner->start);
    text[scanner->current - scanner->start] = '\0';

    scannerAddToken(list, scanner, source, text, NUMBER);
    free(text);
}

bool scannerIsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool scannerIsAlphaNumeric(char c) {
    return scannerIsAlpha(c) || scannerIsDigit(c);
}

void scannerIdentifier(TokenList **list, Scanner *scanner, char *source) {
    while (scannerIsAlphaNumeric(scannerPeek(*scanner, source)))
        scannerAdvance(scanner, source);

    char *text = (char *)malloc(scanner->current - scanner->start + 1);
    memcpy(text, source + scanner->start, scanner->current - scanner->start);
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

    free(text);
    scannerAddToken(list, scanner, source, NULL, type);
}

#endif // SCANNER_IMPLEMENTATION
