#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define STR_LENGTH 100
#define TOKEN_LENGTH 10

void defineAst(char *path, char class_names_upper[][STR_LENGTH],
               char class_names_lower[][STR_LENGTH],
               char class_types[][STR_LENGTH], size_t count) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("Error: could not open file: %s\n", path);
        return;
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
    fprintf(file, "typedef struct Expr {\n\tExprType type;\n\tunion {\n");
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

    // Write the function headers
    fprintf(file, "void clearExpr(Expr *expr);\n");
    fprintf(file, "void visitExpr(Expr *expr, uint level);\n");
    for (size_t i = 0; i < count; i++) {
        fprintf(file, "void visit%c%s(Expr *expr, uint level);\n",
                class_names_upper[i][0],
                class_names_lower[i] + 1 * sizeof(char));
    }

    fprintf(file, "\n#endif // EXPR_H\n\n");

    fclose(file);
}

void defineVisitorFunctinos(char *path, char class_names_upper[][STR_LENGTH],
                            char class_names_lower[][STR_LENGTH],
                            size_t count) {
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        printf("Error: could not open file: %s\n", path);
        return;
    }

    // Header guards
    fprintf(file, "#ifdef EXPR_IMPLEMENTATION\n");
    fprintf(file, "#undef EXPR_IMPLEMENTATION\n\n");

    // Write the switch function
    fprintf(file, "void visitExpr(Expr *expr, uint level) {\n");
    fprintf(file, "\tswitch (expr->type) {\n");
    for (size_t i = 0; i < count; i++) {
        fprintf(file, "\t\tcase EXPR_%s:\n", class_names_upper[i]);
        fprintf(file, "\t\t\tvisit%c%s(expr);\n", class_names_upper[i][0],
                class_names_lower[i] + 1 * sizeof(char));
        fprintf(file, "\t\t\tbreak;\n");
    }
    fprintf(file, "\t}");
    fprintf(file, "\n}\n\n");
    fprintf(
        file,
        "// Do not copy this, as it will overwrite the implementation.\n\n");

    for (size_t i = 0; i < count; i++) {
        fprintf(file, "void visit%c%s(Expr *expr, uint level) {\n",
                class_names_upper[i][0],
                class_names_lower[i] + 1 * sizeof(char));
        fprintf(file, "}\n\n");
    }

    fprintf(file, "#endif // EXPR_IMPLEMENTATION\n\n");
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: expr_util <output directory>\n");
        return 1;
    }

    char *output_dir = argv[1];
    char *name = "expr_structs";
    char path[sizeof(output_dir) + sizeof(name) + 6] = "../";
    strcat(path, output_dir);
    strcat(path, "/");
    strcat(path, name);
    strcat(path, ".h");
    printf("Output directory path: %s\n", path);

    size_t expr_class_count = 4;
    char ast_classes[][STR_LENGTH] = {
        "GROUPING-Expr grouping",
        "BINARY-Expr *left, Token token, Expr *right",
        /*"LITERAL-Token token, Literal *literal",*/
        "LITERAL-Token token",
        "UNARY-Token token, Expr *right",
    };

    // Spilt into the class and the types
    char class_names_upper[expr_class_count][STR_LENGTH];
    char class_names_lower[expr_class_count][STR_LENGTH];
    char class_types[expr_class_count][STR_LENGTH];
    for (size_t i = 0; i < expr_class_count; i++) {
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

    defineAst(path, class_names_upper, class_names_lower, class_types,
              expr_class_count);
    defineVisitorFunctinos(path, class_names_upper, class_names_lower,
                           expr_class_count);

    return 0;
}
