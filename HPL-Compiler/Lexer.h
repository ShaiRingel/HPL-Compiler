#pragma once
#include "Global.h"
#include "LexerFSM.h"
#include <stdio.h>

typedef struct {
	char* lexeme;
	TokenType type;
} Token;

typedef struct {
	LexerFSM* lexerFSM;
    FILE* fp;
} Lexer;

// Initialize lexer
Lexer* lexerInit(char* path);
// Gets the next token
void nextToken(Lexer* lexer);
