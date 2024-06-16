#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STR_LENGTH 100
#define TOKEN_LENGTH 10

void defineAst(char *output_dir, char *name, char ast_classes[][STR_LENGTH],
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

    // Spilt into the class and the types
    char class_names_upper[count][STR_LENGTH];
    char class_names_lower[count][STR_LENGTH];
    char class_types[count][STR_LENGTH];
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

    fprintf(file,
            "#ifndef EXPR_H\n#define EXPR_H\n\n#include \"tokens.h\"\n\n// "
            "Generated this with code\n");

    // Write the enum
    fprintf(file, "typedef enum {\n");
    for (size_t i = 0; i < count; i++) {
        fprintf(file, "\tEXPR_%s", class_names_upper[i]);
        if (i != count - 1)
            fprintf(file, ",");
        fprintf(file, "\n");
    }
    fprintf(file, "} ExprType;\n\n");

    // Write the struct
    fprintf(file, "typedef struct {\n\tExprType type;\n\tunion {\n");
    for (size_t i = 0; i < count; i++) {
        // Special Grouping case
        if (strcmp(class_names_lower[i], "grouping") == 0) {
            fprintf(file, "\t\tstruct Expr *grouping;\n");
            continue;
        }

        // Other cases
        fprintf(file, "\t\tstruct {\n");

        // Get the tokens
        char tokens[TOKEN_LENGTH][STR_LENGTH];
        char *token = strtok(class_types[i], ", ");
        size_t j = 0;
        while (token != NULL) {
            strcpy(tokens[j], token);
            token = strtok(NULL, ", ");
            j++;
        }

        // Parse the tokens
        for (size_t h = 0; h < j; h += 2) {
            // Special Object case
            fprintf(file, "\t\t\t");
            if (strcmp("Expr", tokens[h]) == 0) {
                fprintf(file, "struct ");
            } else if (strcmp("Literal", tokens[h]) == 0) {
                fprintf(file, "union ");
            }
            fprintf(file, "%s %s;\n", tokens[h], tokens[h + 1]);
        }

        fprintf(file, "\t\t} %s;\n", class_names_lower[i]);
    }
    fprintf(file, "\t} value;\n} Expr;\n\n");

    fprintf(file, "#endif // EXPR_H\n\n");

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
    char ast_classes[][STR_LENGTH] = {
        "GROUPING-Expr grouping",
        "BINARY-Expr *left, Token *token, Expr *right",
        "LITERAL-Token *token, Literal *literal",
        "UNARY-Token *token, Expr *right",
    };

    defineAst(output_dir, "expr_structs", ast_classes, expr_class_count);

    return 0;
}
