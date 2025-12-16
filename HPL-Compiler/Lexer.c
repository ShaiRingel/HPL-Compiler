#include "Lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void initLexer(Lexer* lexer, char** sourceFile, int numRows) {
    lexer->inputBuffer = sourceFile;
    lexer->numRows = numRows;
    lexer->currentRow = 0;
    lexer->currentCol = 1;
    lexer->currentChar = sourceFile[0][0];
}


void nextChar(Lexer* lexer) {
    if (lexer->currentRow >= lexer->numRows) {
        lexer->currentChar = '\0';
        return;
    }

    lexer->currentChar = lexer->inputBuffer[lexer->currentRow][lexer->currentCol];

    if (lexer->currentChar == '\0') {
        lexer->currentRow++;
        lexer->currentCol = 0;
        nextChar(lexer);
    } else {
        lexer->currentCol++;
    }
}

void skipWhiteSpaces(Lexer* lexer) {
    while (isspace(lexer->inputBuffer[lexer->currentRow][lexer->currentCol]))
        lexer->currentCol++;
}


void skipComments(Lexer *lexer) {
    char* lexeme = &lexer->inputBuffer[lexer->currentRow][lexer->currentCol - 1];
    if (!strncmp(lexeme, "NOTE:", 5)) {
        int currRow = lexer->currentRow;
        do {
            nextChar(lexer);
        } while (lexer ->currentRow == currRow);
    }
}

int isKeyword(Lexer *lexer) {
    char* lexeme = &lexer->inputBuffer[lexer->currentRow][lexer->currentCol - 1];
    if (!strncmp(lexeme, "Let", 3))
        return TOKEN_LET;
    if (!strncmp(lexeme, "be", 2))
        return TOKEN_BE;
    if (!strncmp(lexeme, "Set", 3))
        return TOKEN_SET;
    if (!strncmp(lexeme, "to", 2))
        return TOKEN_TO;
    if (!strncmp(lexeme, "integer", 7))
        return TOKEN_INTEGER;
    if (!strncmp(lexeme, "Show", 4))
        return TOKEN_SHOW;
    if (!strncmp(lexeme, "Increase", 8))
        return TOKEN_INCREASE;
    if (!strncmp(lexeme, "Decrease", 8))
        return TOKEN_DECREASE;
    if (!strncmp(lexeme, "by", 2))
        return TOKEN_BY;

    return 0;
}


char* getFullLexeme(Lexer* lexer, int (*condition)(int c)) {
    char *lexeme, *tmp;
    int size = 0;

    lexeme = (char*)malloc(2 * sizeof(char));
    if (!lexeme) {
        free(lexeme);
        perror("Memory overflow");
        exit(EXIT_FAILURE);
    }

    do {
        tmp = realloc(lexeme, (++size + 1) * sizeof(char));
        if (!tmp) {
            free(lexeme);
            perror("Memory overflow");
            exit(EXIT_FAILURE);
        }
        lexeme = tmp;

        lexeme[size - 1] = lexer->currentChar;
        nextChar(lexer);
    } while (condition(lexer->currentChar));

    lexeme[size] = '\0';
    return lexeme;
}

Token tokenize(Lexer *lexer) {
    skipWhiteSpaces(lexer); skipComments(lexer);
    Token token;

    if (isdigit(lexer->currentChar) || 
            (lexer->currentChar == '-' && 
            isdigit(lexer->inputBuffer[lexer->currentRow][lexer->currentCol + 1])))
        token.type = TOKEN_NUMBER;
    else if (isKeyword(lexer))
        token.type = isKeyword(lexer);
    else
        token.type = TOKEN_IDENT;

    if (token.type == TOKEN_NUMBER)
        token.lexeme = getFullLexeme(lexer, numberCondition);
    else
        token.lexeme = getFullLexeme(lexer, wordCondition);

    nextChar(lexer);
    return token;
}

int numberCondition(int c) {
    return isdigit(c);
}

int wordCondition(int c) {
    return !isspace(c) && c != '.';
}
