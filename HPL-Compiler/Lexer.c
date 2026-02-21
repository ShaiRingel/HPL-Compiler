#define _CRT_SECURE_NO_WARNINGS
#include "Lexer.h"
#include "LexerFSM.h"
#include <stdlib.h>
#include <ctype.h>

void lexemeSizeExpander(Token *token, int *capacity) {
	char* temp;
	
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

Token nextToken(Lexer* lexer) {
	int counter, capacity;
	char c, *temp;
	Token token;

	counter = 0;
	capacity = LONGEST_WORD_LENGTH + 1;
	c = fgetc(lexer->fp);

	token.type = TOKEN_IDLE;
	token.lexeme = (char*)malloc(capacity * sizeof(char));

	while (token.type == TOKEN_IDLE) {
		token.lexeme[counter++] = c;
		token.type = advance(lexer->lexerFSM, c);

		if (isspace(c)) counter--;

		if (counter >= capacity)
			lexemeSizeExpander(&token, &capacity);

		c = fgetc(lexer->fp);
	}

	if (c != EOF)
		fseek(lexer->fp, -1, SEEK_CUR);

	token.lexeme[counter] = 0;
	token.lexeme = (char*) realloc(token.lexeme, (counter + 1) * sizeof(char));

	return token;
}