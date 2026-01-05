#include "FileReader.h"
#include "Lexer.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>

#define MAIN_FILE_NAME "Main.HPL"

void LexerTest();
void ParserTest();

int main() {
	puts("Lexer test:");
	LexerTest();
	puts("\n\nParser test:");
	ParserTest();

	return 0;
}

void LexerTest() {
	FileDetails fileDetails = readFile(MAIN_FILE_NAME);
	Lexer lexer;

	initLexer(&lexer, fileDetails);

	do {
		Token token = tokenize(&lexer);
		printf("%s - %d\n", token.lexeme, token.type);
	} while (lexer.currentRow < lexer.fileDetails.rowsNum);
}


void ParserTest() {
	FileDetails fileDetails = readFile(MAIN_FILE_NAME);
	Lexer lexer;
	Parser parser;

	program(&parser, fileDetails);
}