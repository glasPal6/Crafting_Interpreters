#ifndef PARSER_H
#define PARSER_H

#include "expr.h"
#include "logging.h"
#include "tokenList.h"
#include <stdbool.h>

typedef struct {
    int current;
} Parser;

Expr parseTokens(TokenList **tokens, bool *had_error);

Expr expression(Parser *parser, TokenList **tokens, bool *had_error);
Expr equality(Parser *parser, TokenList **tokens, bool *had_error);
Expr comparison(Parser *parser, TokenList **tokens, bool *had_error);
Expr term(Parser *parser, TokenList **tokens, bool *had_error);
Expr factor(Parser *parser, TokenList **tokens, bool *had_error);
Expr unary(Parser *parser, TokenList **tokens, bool *had_error);
Expr primary(Parser *parser, TokenList **tokens, bool *had_error);

bool parserMatch(Parser *parser, TokenList **tokens, TokenType *types,
                 size_t count);
bool parserCheck(Parser *parser, TokenList **tokens, TokenType type);
Token parserAdvance(Parser *parser, TokenList **tokens);
bool parserIsAtEnd(Parser *parser, TokenList **tokens);
Token parserPeek(Parser *parser, TokenList **tokens);
Token parserPrevious(Parser *parser, TokenList **tokens);

void parserConsume(Parser *parser, TokenList **tokens, TokenType type,
                   char *message, bool *had_error);
void parserSynchronization(Parser *parser, TokenList **tokens);

#endif // !PARSER_H

#ifdef PARSER_IMPLEMENTATION
#undef PARSER_IMPLEMENTATION

Expr parseTokens(TokenList **tokens, bool *had_error) {
    Parser parser = {.current = 0};
    Expr expr = expression(&parser, tokens, had_error);
    // expr is EOF if had_error is true
    return expr;
}

Expr expression(Parser *parser, TokenList **tokens, bool *had_error) {
    Expr expr = equality(parser, tokens, had_error);
    return expr;
}

Expr equality(Parser *parser, TokenList **tokens, bool *had_error) {
    Expr expr = comparison(parser, tokens, had_error);

    TokenType checkTokens[2] = {BANG_EQUAL, EQUAL_EQUAL};
    while (parserMatch(parser, tokens, checkTokens, 2)) {
        Token op = parserPrevious(parser, tokens);
        Expr right = comparison(parser, tokens, had_error);
        Expr old_expr = {.type = expr.type, .value = expr.value};
        expr = (Expr){.type = EXPR_BINARY,
                      .value.binary = {.left = (struct Expr *)&old_expr,
                                       .token = op,
                                       .right = (struct Expr *)&right}};
    }

    return expr;
}

Expr comparison(Parser *parser, TokenList **tokens, bool *had_error) {
    Expr expr = term(parser, tokens, had_error);

    TokenType checkTokens[4] = {GREATER, GREATER_EQUAL, LESS, LESS_EQUAL};
    while (parserMatch(parser, tokens, checkTokens, 4)) {
        Token op = parserPrevious(parser, tokens);
        Expr right = term(parser, tokens, had_error);
        Expr old_expr = {.type = expr.type, .value = expr.value};
        expr = (Expr){.type = EXPR_BINARY,
                      .value.binary = {.left = (struct Expr *)&old_expr,
                                       .token = op,
                                       .right = (struct Expr *)&right}};
    }

    return expr;
}

Expr term(Parser *parser, TokenList **tokens, bool *had_error) {
    Expr expr = factor(parser, tokens, had_error);

    TokenType checkTokens[2] = {MINUS, PLUS};
    while (parserMatch(parser, tokens, checkTokens, 2)) {
        Token op = parserPrevious(parser, tokens);
        Expr right = factor(parser, tokens, had_error);
        Expr old_expr = {.type = expr.type, .value = expr.value};
        expr = (Expr){.type = EXPR_BINARY,
                      .value.binary = {.left = (struct Expr *)&old_expr,
                                       .token = op,
                                       .right = (struct Expr *)&right}};
    }

    return expr;
}

Expr factor(Parser *parser, TokenList **tokens, bool *had_error) {
    Expr expr = unary(parser, tokens, had_error);

    TokenType checkTokens[2] = {SLASH, STAR};
    while (parserMatch(parser, tokens, checkTokens, 2)) {
        Token op = parserPrevious(parser, tokens);
        Expr right = unary(parser, tokens, had_error);
        Expr old_expr = {.type = expr.type, .value = expr.value};
        expr = (Expr){.type = EXPR_BINARY,
                      .value.binary = {.left = (struct Expr *)&old_expr,
                                       .token = op,
                                       .right = (struct Expr *)&right}};
    }

    return expr;
}

Expr unary(Parser *parser, TokenList **tokens, bool *had_error) {
    TokenType checkTokens[2] = {BANG, MINUS};
    if (parserMatch(parser, tokens, checkTokens, 2)) {
        Token op = parserPrevious(parser, tokens);
        Expr right = unary(parser, tokens, had_error);
        return (Expr){
            .type = EXPR_UNARY,
            .value.unary = {.token = op, .right = (struct Expr *)&right}};
    }

    Expr expr = primary(parser, tokens, had_error);

    return expr;
}

Expr primary(Parser *parser, TokenList **tokens, bool *had_error) {
    TokenType checkToken[2] = {NIL, NIL};

    checkToken[0] = FALSE;
    if (parserMatch(parser, tokens, checkToken, 1)) {
        Token op = (Token){.type = FALSE,
                           .lexeme = "false",
                           .literal.string = "false",
                           .line = -1};
        Expr expr = {.type = EXPR_LITERAL, .value.literal = {.token = op}};
        return expr;
    }
    checkToken[0] = TRUE;
    if (parserMatch(parser, tokens, checkToken, 1)) {
        Token op = (Token){.type = TRUE,
                           .lexeme = "true",
                           .literal.string = "true",
                           .line = -1};
        Expr expr = {.type = EXPR_LITERAL, .value.literal = {.token = op}};
        return expr;
    }
    checkToken[0] = NIL;
    if (parserMatch(parser, tokens, checkToken, 1)) {
        Token op = (Token){
            .type = NIL, .lexeme = "", .literal.string = "", .line = -1};
        Expr expr = {.type = EXPR_LITERAL, .value.literal = {.token = op}};
        return expr;
    }

    checkToken[0] = NUMBER;
    checkToken[1] = STRING;
    if (parserMatch(parser, tokens, checkToken, 2)) {
        Token tokenLiteral = parserPrevious(parser, tokens);
        Expr expr = {.type = EXPR_LITERAL,
                     .value.literal = {.token = tokenLiteral}};
        return expr;
    }

    checkToken[0] = LEFT_PAREN;
    if (parserMatch(parser, tokens, checkToken, 1)) {
        Expr expr = expression(parser, tokens, had_error);
        parserConsume(parser, tokens, RIGHT_PAREN,
                      "Expect ')' after expression.", had_error);
        Expr expr_grouping = {.type = EXPR_GROUPING,
                              .value.grouping = (struct Expr *)&expr};
        return expr_grouping;
    }

    errorToken(parserPeek(parser, tokens), "Expected Expresions.", had_error);

    // Check this
    Token null_literal = {
        .type = EOF_I, .lexeme = "", .literal.string = "", .line = -1};
    Expr expr = {.type = EXPR_LITERAL,
                 .value.literal = {.token = null_literal}};
    return expr;
}

bool parserMatch(Parser *parser, TokenList **tokens, TokenType *types,
                 size_t count) {
    for (size_t i = 0; i < count; i++) {
        bool check_token_type = parserCheck(parser, tokens, types[i]);
        if (check_token_type) {
            parserAdvance(parser, tokens);
            return true;
        }
    }

    return false;
}

bool parserCheck(Parser *parser, TokenList **tokens, TokenType type) {
    if (parserIsAtEnd(parser, tokens))
        return false;
    return parserPeek(parser, tokens).type == type;
}

Token parserAdvance(Parser *parser, TokenList **tokens) {
    if (!parserIsAtEnd(parser, tokens))
        parser->current++;
    return parserPeek(parser, tokens);
}

bool parserIsAtEnd(Parser *parser, TokenList **tokens) {
    return parserPeek(parser, tokens).type == EOF_I;
}

Token parserPeek(Parser *parser, TokenList **tokens) {
    return tokensListIndexOf(tokens, parser->current);
}

Token parserPrevious(Parser *parser, TokenList **tokens) {
    return tokensListIndexOf(tokens, parser->current - 1);
}

void parserConsume(Parser *parser, TokenList **tokens, TokenType type,
                   char *message, bool *had_error) {
    if (parserCheck(parser, tokens, type)) {
        parserAdvance(parser, tokens);
    }

    errorToken(parserPeek(parser, tokens), message, had_error);
}

void parserSynchronization(Parser *parser, TokenList **tokens) {
    parserAdvance(parser, tokens);

    while (!parserIsAtEnd(parser, tokens)) {
        if (parserPrevious(parser, tokens).type == SEMICOLON)
            return;

        switch (parserPeek(parser, tokens).type) {
        case CLASS:
        case FUN:
        case VAR:
        case FOR:
        case IF:
        case WHILE:
        case PRINT:
        case RETURN:
            return;
        default:
            parserAdvance(parser, tokens);
        }
    }
}

#endif // PARSER_IMPLEMENTATION
