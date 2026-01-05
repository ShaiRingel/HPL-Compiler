#define _CRT_SECURE_NO_WARNINGS
#include "Lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


void initLexer(Lexer* lexer, FileDetails fileDetails) {
    lexer->fileDetails = fileDetails;
    lexer->currentRow = 0;
    lexer->currentCol = 1;
    lexer->currentChar = (fileDetails.rowsNum == 0 ? '\0' :
                            fileDetails.inputBuffer[0][0]);
}


void nextChar(Lexer* lexer) {
    if (lexer->currentRow >= lexer->fileDetails.rowsNum) {
        lexer->currentChar = '\0';
        return;
    }

    lexer->currentChar = 
        lexer->fileDetails.inputBuffer[lexer->currentRow][lexer->currentCol++];

    if (lexer->currentChar == '\0') {
        lexer->currentRow++;
        lexer->currentCol = 0;
        nextChar(lexer);
    }
}


void skipWhiteSpaces(Lexer* lexer) {
    while (lexer->currentChar != '\0' && isspace(lexer->currentChar))
        nextChar(lexer);
}


void skipComments(Lexer* lexer) {
    if (lexer->currentChar == '\0')
        return;

    char* lexeme = &lexer->fileDetails.inputBuffer[lexer->currentRow][lexer->currentCol - 1];

    if (!strncmp(lexeme, COMMENT, strlen(COMMENT))) {
        int curRow = lexer->currentRow;
        while (curRow == lexer->currentRow && lexer->currentChar != END_OF_LINE)
            nextChar(lexer);
    }
}


int isKeyword(const char* word) {
    int i, size = (sizeof(keywordTable) / sizeof(keywordTable[0]));
    for (i = 0; i < size; i++)
        if (!strcmp(word, keywordTable[i].keyword))
            return keywordTable[i].type;

    return TOKEN_IDENT;
}


char* getFullLexeme(Lexer* lexer, int (*condition)(int)) {
    char* lexeme, * tmp;
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


Token tokenize(Lexer* lexer) {
    Token token;

    do {
        skipWhiteSpaces(lexer);
        skipComments(lexer);
    } while (isspace(lexer->currentChar));

    if (lexer->currentChar == '\0') {
        token.type = TOKEN_EOF;
        token.lexeme = _strdup("");
        return token;
    }

    if (isdigit(lexer->currentChar) ||
        (lexer->currentChar == '-' &&
            isdigit(
                lexer->fileDetails.inputBuffer
                [lexer->currentRow][lexer->currentCol + 1]))) {
        token.type = TOKEN_NUMBER;
        token.lexeme = getFullLexeme(lexer, numberCondition);
    }
    else {
        token.lexeme = getFullLexeme(lexer, wordCondition);
        token.type = isKeyword(token.lexeme);
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