#ifndef EXPR_H
#define EXPR_H

#include "tokens.h"

// Generated this with code
typedef enum {
	EXPR_GROUPING,
	EXPR_BINARY,
	EXPR_LITERAL,
	EXPR_UNARY
} ExprType;

typedef struct {
	ExprType type;
	union {
		struct Expr *grouping;
		struct {
			struct Expr *left;
			Token *token;
			struct Expr *right;
		} binary;
		struct {
			Token *token;
			union Literal *literal;
		} literal;
		struct {
			Token *token;
			struct Expr *right;
		} unary;
	} value;
} Expr;

void visitExpr(Expr *expr);
void visitGrouping(Expr *expr);
void visitBinary(Expr *expr);
void visitLiteral(Expr *expr);
void visitUnary(Expr *expr);

#endif // EXPR_H

#ifdef EXPR_IMPLEMENTATION
#undef EXPR_IMPLEMENTATION

void visitExpr(Expr *expr) {
	switch (expr->type) {
		case EXPR_GROUPING:
			visitGrouping(expr);
			break;
		case EXPR_BINARY:
			visitBinary(expr);
			break;
		case EXPR_LITERAL:
			visitLiteral(expr);
			break;
		case EXPR_UNARY:
			visitUnary(expr);
			break;
	}
}

void visitGrouping(Expr *expr) {
}

void visitBinary(Expr *expr) {
}

void visitLiteral(Expr *expr) {
}

void visitUnary(Expr *expr) {
}

#endif // EXPR_IMPLEMENTATION

