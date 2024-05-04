#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdio.h>
#include <stdlib.h>

void error(int line, char *message);
void report(int line, char *where, char* message);

#endif // _LOGGING_H

//Clangd hack
#ifndef LOGGING_IMPLEMENTATION_MAIN
#define LOGGING_IMPLEMENTATION
#endif // !LOGGING_IMPLEMENTATION_MAIN
//Clangd hack

#ifdef LOGGING_IMPLEMENTATION

void error(int line, char *message)
{
    report(line, "", message);
}

void report(int line, char *where, char *message)
{
    printf("[line %d] Error %s: %s\n", line, where, message);
    exit(1);
}

#endif // LOGGING_IMPLEMENTATION
