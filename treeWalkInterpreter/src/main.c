#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "logging.h"
#include "scanner.h"
#include "tokenList.h"

void interpret(char *source) {
    bool had_error = false;

    TokenList *tokens = NULL;

    scanTokens(&tokens, source, &had_error);
    /*listPrint(tokens);*/

    while (tokens != NULL) {
        listPop(&tokens);
    }

    if (had_error)
        exit(1);
}

void runFile(char *path) {
    FILE *lox_file = fopen(path, "r");
    if (lox_file == NULL) {
        printf("Error: could not open file: %s\n", path);
        exit(1);
    }

    // Get the file size
    fseek(lox_file, 0L, SEEK_END);
    size_t file_size = ftell(lox_file);
    rewind(lox_file);

    // Load the file as a string
    char *buffer = (char *)malloc(file_size + 1);
    size_t bytes_to_read = fread(buffer, sizeof(char), file_size, lox_file);
    if (bytes_to_read < file_size) {
        printf("Error: could not read the file: %s\n", path);
        exit(1);
    }

    buffer[bytes_to_read] = '\0';
    fclose(lox_file);

    interpret(buffer);

    free(buffer);
    exit(0);
}

void runPrompt() {
    char line[1024];

    while (1) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }

    exit(1);
}

int main(int argc, char *argv[]) {
    // Check the args for the file
    if (argc > 2) {
        printf("Usage: jlox [script]\n");
        return 1;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }

    return 0;
}
