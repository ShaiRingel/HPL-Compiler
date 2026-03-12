#define _CRT_SECURE_NO_WARNINGS
#include "Lexer.h"
#include "LexerFSM.h"
#include <stdlib.h>
#include <ctype.h>

void lexemeSizeExpander(Token *token, int *capacity) {
	char* temp;
	if (*capacity & 8000 != 0) {
		printf(RED "Error: Maximum identifier name size reached!\n" RESET);
		exit(EXIT_FAILURE);
	}

	(*capacity) <<= 1;
	temp = (char*)realloc(token->lexeme, *capacity * sizeof(char));

	if (!temp) {
		exit(EXIT_FAILURE);
	}

	token->lexeme = temp;
}

Lexer* initLexer(char* path) {
	Lexer* lexer = (Lexer*) malloc(sizeof(Lexer));
	if (!lexer) {
		printf(RED "Error: Failed to allocate memory for lexer\n" RESET);
		exit(EXIT_FAILURE);
	}

	lexer->lexerFSM = initLexerFSM();
	lexer->fp = fopen(path, "r");

	return lexer;
}

Token* nextToken(Lexer* lexer) {
	LexerFSM* lexerFSM = lexer->lexerFSM;
	int counter, capacity;
	unsigned short* curr;
	char c, *temp;
	Token* token = (Token*) malloc(sizeof(Token));

	capacity = LONGEST_WORD_LENGTH + 1;
	curr = &lexerFSM->currentState;
	lexerFSM->currentState = 0;
	c = fgetc(lexer->fp);
	counter = 0;

	token->type = TOKEN_IDLE;
	token->lexeme = (char*)malloc(capacity * sizeof(char));

	while (*curr != STATE_ACCEPT) {
		token->lexeme[counter++] = c;
		token->type = advance(lexerFSM, c);

		counter -= token->type != TOKEN_IDLE ||
			*curr == STATE_COMMENT;
		
		if (token->type == TOKEN_IDLE && !*curr)
			counter = 0;

		if (counter >= capacity)
			lexemeSizeExpander(token, &capacity);
		
		if (token->type == TOKEN_IDLE)
			c = fgetc(lexer->fp);
	}

	ungetc(c, lexer->fp);

	token->lexeme[counter] = 0;
	token->lexeme = (char*) realloc(token->lexeme, (counter + 1) * sizeof(char));

	return token;
}

void freeLexer(Lexer* lexer) {
	freeLexerFSM(lexer->lexerFSM);
	free(lexer);
}