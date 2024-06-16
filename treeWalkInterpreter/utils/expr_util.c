#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void defineAst(char *output_dir, char *name, char ast_classes[][100],
               size_t count) {
    char path[sizeof(output_dir) + sizeof(name) + 6] = "../";
    strcat(path, output_dir);
    strcat(path, "/");
    strcat(path, name);
    strcat(path, ".h");
    printf("Generating: %s\n", path);

    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("Error: could not open file: %s\n", path);
        return;
    }

    char class_names_upper[count][100];
    char class_names_lower[count][100];
    char class_types[count][100];
    for (size_t i = 0; i < count; i++) {
        char *tokens = strtok(ast_classes[i], "-");
        strcpy(class_names_upper[i], tokens);
        strcpy(class_names_lower[i], tokens);
        char *lower = class_names_lower[i];
        while (*lower) {
            *lower = tolower((unsigned char)*lower);
            lower++;
        }
        tokens = strtok(NULL, ":");
        strcpy(class_types[i], tokens);
    }

    // Write the first part
    fprintf(file,
            "#ifndef EXPR_H\n#define EXPR_H\n\n#include \"tokens.h\"\n\n// "
            "Generated this with code\ntypedef enun { ");
    for (size_t i = 0; i < count; i++) {
        fprintf(file, "EXPR_%s, ", ast_classes[i]);
    }
    fprintf(file, "} ExprType;\n\n");

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: expr_util <output directory>\n");
        return 1;
    }

    char *output_dir = argv[1];
    printf("Output directory: %s\n", output_dir);

    size_t expr_class_count = 4;
    char ast_classes[][100] = {
        "BINARY-Expr left, Token operator, Expr right",
        "GROUPING-Expr expression",
        "LITERAL-Object value",
        "UNARY-Token operator, Expr right",
    };

    defineAst(output_dir, "expr_structs", ast_classes, expr_class_count);

    return 0;
}
