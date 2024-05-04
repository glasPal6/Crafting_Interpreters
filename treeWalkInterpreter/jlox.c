#include <stdio.h>

int main(int argc, char *argv[])
{
    // Check the args for the file
    if (argc > 2) {
        printf("Usage: jlox [script]\n");
        return 1;
    } else if (argc == 2) {
        printf("runFile: %s\n", argv[1]);
    } else {
        printf("runPrompt\n");
    }

    return 0;
}
