#ifndef _LOGGING_H
#define _LOGGING_H

void error(int line, char *message);
void report(int line, char *where, char* message);

#endif // _LOGGING_H

#define LOGGING_IMPLEMENTATION
#ifdef LOGGING_IMPLEMENTATION

void error(int line, char *message)
{
    report(line, ' ', message);
}

void report(int line, char *where, char* message)
{
}

#endif // LOGGING_IMPLEMENTATION
