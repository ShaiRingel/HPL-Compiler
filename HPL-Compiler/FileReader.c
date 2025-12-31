#define _CRT_SECURE_NO_WARNINGS
#include "FileReader.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


FileDetails readFile(char* path) {
    FILE* fileptr = fopen(path, "r");
    if (!fileptr) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    FileDetails fileDetails;
    char buffer[256];
    int size = 0;

    fileDetails.inputBuffer = NULL;
    while (fgets(buffer, sizeof(buffer), fileptr)) {
        int i;
        char** tmp = (char**)realloc(fileDetails.inputBuffer, (size + 1) * sizeof(char*));
        buffer[strcspn(buffer, "\n")] = '\0';

        if (!tmp) {
            perror("Memory overflow");
            for (i = 0; i < size; i++) free(fileDetails.inputBuffer[i]);
            free(fileDetails.inputBuffer);
            fclose(fileptr);
            exit(EXIT_FAILURE);
        }
        fileDetails.inputBuffer = tmp;

        fileDetails.inputBuffer[size] = malloc(strlen(buffer) + 1);
        if (!fileDetails.inputBuffer[size]) {
            perror("Memory overflow");
            for (i = 0; i < size; i++) free(fileDetails.inputBuffer[i]);
            free(fileDetails.inputBuffer);
            fclose(fileptr);
            exit(EXIT_FAILURE);
        }
        strcpy(fileDetails.inputBuffer[size], buffer);
        size++;
    }

    fileDetails.rowsNum = size;

    return fileDetails;
}