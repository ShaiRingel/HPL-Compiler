#define _CRT_SECURE_NO_WARNINGS
#include "Lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


void initLexer(Lexer* lexer, FileDetails fileDetails) {
    lexer->fileDetails = fileDetails;
    lexer->currentRow = 0;
    lexer->currentCol = 1;
    lexer->currentChar = fileDetails.inputBuffer[0][0];
}


void nextChar(Lexer* lexer) {
    if (lexer->currentRow >= lexer->fileDetails.rowsNum) {
        lexer->currentChar = '\0';
        return;
    }

    lexer->currentChar = lexer->fileDetails.inputBuffer[lexer->currentRow][lexer->currentCol];

    if (lexer->currentChar == END_OF_LINE) {
        lexer->currentRow++;
        lexer->currentCol = 0;
    } else {
        lexer->currentCol++;
    }
}


void skipWhiteSpaces(Lexer* lexer) {
    while (isspace(lexer->currentChar)) {
        nextChar(lexer);
    }
}


void skipComments(Lexer *lexer) {
    char* lexeme = &lexer->fileDetails.inputBuffer[lexer->currentRow][lexer->currentCol - 1];
    if (!strncmp(lexeme, COMMENT, 5)) {
        int currRow = lexer->currentRow;
        do {
            nextChar(lexer);
        } while (lexer->currentRow == currRow);
    }
}


int isKeyword(Lexer *lexer) {
    int i, c;
    char *lexeme = &lexer->fileDetails.inputBuffer[lexer->currentRow][lexer->currentCol - 1];
    
    for (c = 0; lexeme[c] != '.' && !isspace(lexeme[c]); c++);
    char* word = (char*) malloc(c + 1);
    if (!word) {
        perror("");
        exit(EXIT_FAILURE);
    }
    strncpy(word, lexeme, c);
    word[c] = '\0';

    for (i = 0; i < sizeof(keywordTable) / sizeof(keywordTable[0]); i++) {
        if (strcmp(word, keywordTable[i].keyword) == 0) {
            free(word);
            return keywordTable[i].type;
        }
    }

    free(word);
    return TOKEN_IDENT;
}


char* getFullLexeme(Lexer* lexer, int (*condition)(int c)) {
    char *lexeme, *tmp;
    int size = 1;

    lexeme = NULL;

    do {
        tmp = realloc(lexeme, (++size) * sizeof(char));
        if (!tmp) {
            free(lexeme);
            perror("Memory overflow");
            exit(EXIT_FAILURE);
        }
        lexeme = tmp;

        lexeme[size - 2] = lexer->currentChar;
        nextChar(lexer);
    } while (condition(lexer->currentChar));

    lexeme[size - 1] = '\0';
    return lexeme;
}


Token tokenize(Lexer *lexer) {
    skipWhiteSpaces(lexer); skipComments(lexer);
    Token token;

    if (isdigit(lexer->currentChar) ||
        (lexer->currentChar == '-' &&
            isdigit(lexer->fileDetails.inputBuffer[lexer->currentRow][lexer->currentCol + 1]))) {
        token.type = TOKEN_NUMBER;
        token.lexeme = getFullLexeme(lexer, numberCondition);
    }
    else {
        token.type = isKeyword(lexer);
        token.lexeme = getFullLexeme(lexer, wordCondition);
    }

    nextChar(lexer);
    return token;
}


int numberCondition(int c) {
    return isdigit(c);
}


int wordCondition(int c) {
    return !isspace(c) && c != '.';
}