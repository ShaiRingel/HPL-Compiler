#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lexer.h"

int main() {
    FILE* fileptr = fopen("Test.hpl", "r");
    if (!fileptr) {
        perror("Failed to open file");
        return 1;
    }
    Lexer lexer;
    char** source = NULL;
    char buffer[255];
    int size = 0;

    while (fgets(buffer, sizeof(buffer), fileptr)) {
        int i;
        char** tmp = (char**)realloc(source, (size + 1) * sizeof(char*));
        buffer[strcspn(buffer, "\n")] = '\0';

        if (!tmp) {
            perror("reallocation failed");
            for (i = 0; i < size; i++) free(source[i]);
            free(source);
            fclose(fileptr);
            return 1;
        }
        source = tmp;

        source[size] = malloc(strlen(buffer) + 1);
        if (!source[size]) {
            perror("malloc failed");
            for (i = 0; i < size; i++) free(source[i]);
            free(source);
            fclose(fileptr);
            return 1;
        }
        strcpy(source[size], buffer);
        size++;
    }

    initLexer(&lexer, source, size);

	do {
        Token token = tokenize(&lexer);
        printf("%s - %d\n\n", token.lexeme, token.type);
	} while (lexer.currentChar);

	return 0;
}