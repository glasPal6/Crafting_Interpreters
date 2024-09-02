#ifndef LOGGING_H
#define LOGGING_H

#include "tokens.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void error(int line, char *message, bool *had_error);
void report(int line, char *where, char *message, bool *had_error);

#endif // LOGGING_H

#ifdef LOGGING_IMPLEMENTATION
#undef LOGGING_IMPLEMENTATION

void error(int line, char *message, bool *had_error) {
    report(line, "", message, had_error);
}

void errorToken(Token token, char *message, bool *had_error) {
    if (token.type == EOF_I) {
        report(token.line, " at end", message, had_error);
    } else {
        char str[256] = " at '";
        strcat(str, token.lexeme);
        strcat(str, "'");
        report(token.line, str, message, had_error);
    }
}

void report(int line, char *where, char *message, bool *had_error) {
    printf("[line %d] Error %s: %s\n", line, where, message);
    *had_error = true;
}

#endif // LOGGING_IMPLEMENTATION
