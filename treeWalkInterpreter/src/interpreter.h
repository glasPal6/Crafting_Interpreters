#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "tokens.h"
#include <stdint.h>
#include <string.h>

void interpretExpr(Expr *expr);

Literal visitExpr(Expr *expr);
Literal visitGrouping(Expr *expr);
Literal visitBinary(Expr *expr);
Literal visitLiteral(Expr *expr);
Literal visitUnary(Expr *expr);

Literal isTruthy(Literal value);
Literal isEqual(Literal left, Literal right);

bool checkNumberOperand(Literal literal1, Literal literal2);

#endif // !INTERPRETER_H

#ifdef INTERPRETER_IMPLEMENTATION
#undef INTERPRETER_IMPLEMENTATION

void interpretExpr(Expr *expr) {
    Literal value = visitExpr(expr);
    printLiteralObj(value);
}

Literal visitExpr(Expr *expr) {
    switch (expr->type) {
    case EXPR_GROUPING:
        return visitGrouping(expr);
        break;
    case EXPR_BINARY:
        return visitBinary(expr);
        break;
    case EXPR_LITERAL:
        return visitLiteral(expr);
        break;
    case EXPR_UNARY:
        return visitUnary(expr);
        break;
    }

    return (Literal){.type = NONE_LITERAL, .object.number = 0};
}

Literal visitGrouping(Expr *expr) {
    return visitExpr((Expr *)expr->value.grouping);
}

Literal visitBinary(Expr *expr) {
    Literal left = visitExpr((Expr *)expr->value.binary.left);
    Literal right = visitExpr((Expr *)expr->value.binary.right);

    LiteralType type = NONE_LITERAL;
    Literal number = {.type = NUMBER_LITERAL, .object.number = 0};
    Literal boolean = {.type = BOOL_LITERAL, .object.boolean = 0};
    Literal string = {.type = STRING_LITERAL, .object.string = ""};

    switch (expr->value.binary.token.type) {
    case MINUS:
        if (checkNumberOperand(left, right)) {
            type = NUMBER_LITERAL;
            number.object.number = left.object.number - right.object.number;
        }
        break;
    case PLUS:
        if (left.type == STRING_LITERAL && right.type == STRING_LITERAL) {
            type = STRING_LITERAL;
            string.object.string = malloc(strlen(left.object.string) +
                                          strlen(right.object.string) + 1);
            strcat(string.object.string, left.object.string);
            strcat(string.object.string, right.object.string);
            break;
        }

        if (checkNumberOperand(left, right)) {
            type = NUMBER_LITERAL;
            number.object.number = left.object.number + right.object.number;
            break;
        }
        break;
    case SLASH:
        if (checkNumberOperand(left, right)) {
            type = NUMBER_LITERAL;
            number.object.number = left.object.number / right.object.number;
        }
        break;
    case STAR:
        if (checkNumberOperand(left, right)) {
            type = NUMBER_LITERAL;
            number.object.number = left.object.number * right.object.number;
        }
        break;

    case GREATER:
        if (checkNumberOperand(left, right)) {
            type = BOOL_LITERAL;
            boolean.object.boolean = left.object.number > right.object.number;
        }
        break;
    case GREATER_EQUAL:
        if (checkNumberOperand(left, right)) {
            type = BOOL_LITERAL;
            boolean.object.boolean = left.object.number >= right.object.number;
        }
        break;
    case LESS:
        if (checkNumberOperand(left, right)) {
            type = BOOL_LITERAL;
            boolean.object.boolean = left.object.number < right.object.number;
        }
        break;
    case LESS_EQUAL:
        if (checkNumberOperand(left, right)) {
            type = BOOL_LITERAL;
            boolean.object.boolean = left.object.number <= right.object.number;
        }
        break;
    case BANG_EQUAL:
        type = BOOL_LITERAL;
        boolean = isEqual(left, right);
        boolean.object.boolean = !left.object.boolean;
        break;
    case EQUAL_EQUAL:
        type = BOOL_LITERAL;
        boolean = isEqual(left, right);
        boolean.object.boolean = left.object.boolean;
        break;

    default:
        break;
    }

    if (type == NUMBER_LITERAL)
        return number;
    if (type == BOOL_LITERAL)
        return boolean;
    if (type == STRING_LITERAL)
        return string;
    return (Literal){.type = NONE_LITERAL, .object.number = 0};
}

Literal visitLiteral(Expr *expr) { return expr->value.literal.token.literal; }

Literal visitUnary(Expr *expr) {
    Literal right = visitExpr((Expr *)expr->value.unary.right);

    LiteralType type = NONE_LITERAL;
    Literal number = {.type = NUMBER_LITERAL, .object.number = 0};
    Literal boolean = {.type = BOOL_LITERAL, .object.boolean = 0};

    switch (expr->value.unary.token.type) {
    case MINUS:
        if (checkNumberOperand(right, right)) {
            type = NUMBER_LITERAL;
            number.object.number = -right.object.number;
        }
        break;
    case BANG:
        type = BOOL_LITERAL;
        right = isTruthy(right);
        boolean.object.boolean = !right.object.boolean;
        break;
    default:
        break;
    }

    if (type == NUMBER_LITERAL)
        return number;
    if (type == BOOL_LITERAL)
        return boolean;
    return (Literal){.type = NONE_LITERAL, .object.number = 0};
}

Literal isTruthy(Literal value) {
    if (value.type == NONE_LITERAL)
        return (Literal){.type = BOOL_LITERAL, .object.boolean = false};
    if (value.type == BOOL_LITERAL)
        return value;
    return (Literal){.type = BOOL_LITERAL, .object.boolean = true};
}

Literal isEqual(Literal left, Literal right) {
    if (left.type == NONE_LITERAL || right.type == NONE_LITERAL)
        return (Literal){.type = BOOL_LITERAL, .object.boolean = true};

    if (left.type != right.type)
        return (Literal){.type = BOOL_LITERAL, .object.boolean = false};

    if (left.type == STRING_LITERAL && right.type == STRING_LITERAL) {
        int result = strcmp(left.object.string, right.object.string);
        return (Literal){.type = BOOL_LITERAL, .object.boolean = result == 0};
    }

    if (left.type == NUMBER_LITERAL && right.type == NUMBER_LITERAL) {
        return (Literal){.type = BOOL_LITERAL,
                         .object.boolean =
                             left.object.number == right.object.number};
    }

    return (Literal){.type = BOOL_LITERAL, .object.boolean = false};
}

bool checkNumberOperand(Literal literal1, Literal literal2) {
    if (literal1.type == NUMBER_LITERAL && literal2.type == NUMBER_LITERAL)
        return true;
    return false;
}

#endif // !INTERPRETER_IMPLEMENTATION
