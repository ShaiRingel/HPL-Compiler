#pragma once
#include "Global.h"
#include "LexerFSM.h"
#include <stdio.h>

#define LONGEST_WORD_LENGTH 11

typedef struct {
	char* lexeme;
	TokenType type;
} Token;

typedef struct {
	LexerFSM* lexerFSM;
    FILE* fp;
} Lexer;

// Initialize lexer
Lexer* initLexer(char* path);
// Gets the next token
Token nextToken(Lexer* lexer);
