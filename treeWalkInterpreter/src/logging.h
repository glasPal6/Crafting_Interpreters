#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>
#include <stdbool.h>

void error(int line, char *message, bool *had_error);
void report(int line, char *where, char* message, bool *had_error);

#endif // LOGGING_H

// Clangd hack
#ifndef LOGGING_IMPLEMENTATION_MAIN
#define LOGGING_IMPLEMENTATION
#endif // !LOGGING_IMPLEMENTATION_MAIN

#ifdef LOGGING_IMPLEMENTATION
#undef LOGGING_IMPLEMENTATION

void error(int line, char *message, bool *had_error)
{
    report(line, "", message, had_error);
}

void report(int line, char *where, char *message, bool *had_error)
{
    printf("[line %d] Error %s: %s\n", line, where, message);
    *had_error = true;
}

#endif // LOGGING_IMPLEMENTATION
