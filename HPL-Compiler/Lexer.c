#define _CRT_SECURE_NO_WARNINGS

#include "Lexer.h"
#include "LexerFSM.h"
#include <stdlib.h>

Lexer* lexerInit(char* path) {
	Lexer* lexer = (Lexer*) malloc(sizeof(Lexer));
	if (!lexer) {
		printf(RED "Error: Failed to allocate memory for lexer\n" RESET);
		exit(EXIT_FAILURE);
	}

	lexer->lexerFSM = initLexerFSM();
	lexer->fp = fopen(path, "r");

	return lexer;
}

void nextToken(Lexer* lexer) {
	//advance();
}