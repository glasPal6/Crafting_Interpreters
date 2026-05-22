#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "tokens.h"

void error(int32_t line, char* message, bool* had_error);
void report(int32_t line, char* where, char* message, bool* had_error);
void runtimeError(int32_t line, char* message, bool* had_runtime_error);

#endif  // LOGGING_H

#ifdef LOGGING_IMPLEMENTATION
#undef LOGGING_IMPLEMENTATION

void error(int32_t line, char* message, bool* had_error) {
    report(line, "", message, had_error);
}

void errorToken(Token token, char* message, bool* had_error) {
    if (token.type == EOF_I) {
        report(token.line, " at end", message, had_error);
    } else {
        char str[256] = " at '";
        strcat(str, token.lexeme);
        strcat(str, "'");
        report(token.line, str, message, had_error);
    }
}

void report(int32_t line, char* where, char* message, bool* had_error) {
    printf("[line %d] Error %s: %s\n", line, where, message);
    *had_error = true;
}

void runtimeError(int32_t line, char* message, bool* had_runtime_error) {
    printf("[line %d] Error at runtime: %s\n", line, message);
    *had_runtime_error = true;
}

#endif  // LOGGING_IMPLEMENTATION
