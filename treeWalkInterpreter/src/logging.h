#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>
#include <stdio.h>

void error(int line, char *message, bool *had_error);
void report(int line, char *where, char *message, bool *had_error);

#endif // LOGGING_H

#ifdef LOGGING_IMPLEMENTATION
#undef LOGGING_IMPLEMENTATION

void error(int line, char *message, bool *had_error) {
    report(line, "", message, had_error);
}

void report(int line, char *where, char *message, bool *had_error) {
    printf("[line %d] Error %s: %s\n", line, where, message);
    *had_error = true;
}

#endif // LOGGING_IMPLEMENTATION
