#pragma once
#include "Global.h"
#include "LexerFSM.h"
#include <stdio.h>

#define MAX_INDENT_DEPTH 10
#define LONGEST_WORD_LENGTH 11

typedef struct {
	LexerFSM* lexerFSM;
    FILE* fp;
    int indentStack[MAX_INDENT_DEPTH];
    int indentTop;
    int pendingDedents;
    int isStartOfLine;
} Lexer;

// Initializes and allocates a new Lexer
Lexer* initLexer(char* path);

// Retrieves the next token in the source file
Token* nextToken(Lexer* lexer);

// Frees the entire Lexer and all associated memory
void freeLexer(Lexer* lexer);