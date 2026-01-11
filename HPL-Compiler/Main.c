#include "FileReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "AST.h"
#include <stdio.h>

#define MAIN_FILE_NAME "Main.HPL"

void LexerTest();
void ParserTest();

int main() {
	puts("Lexer test:");
	LexerTest();
	puts("\n-------------------------------------------------------");
	puts("\nParser test:");
	ParserTest();

	return 0;
}

void LexerTest() {
	FileDetails fileDetails = readFile(MAIN_FILE_NAME);
	Lexer *lexer = initLexer(fileDetails);

	do {
		Token token = tokenize(lexer);
		printf("%s - %d\n", token.lexeme, token.type);
	} while (lexer->currentRow < lexer->fileDetails.rowsNum);

	freeLexer(lexer);
}


void ParserTest() {
	FileDetails fileDetails = readFile(MAIN_FILE_NAME);
	Lexer lexer;
	Parser parser;

	ASTNode *root = program(fileDetails);

	printAST(root->next, 0);
}