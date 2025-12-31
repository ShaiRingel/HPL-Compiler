#include "FileReader.h"
#include "Lexer.h"
#include <stdio.h>

int main() {
	FileDetails fileDetails = readFile("Test.HPL");
	Lexer lexer;

    initLexer(&lexer, fileDetails);

	do {
        Token token = tokenize(&lexer);
        printf("%s - %d\n", token.lexeme, token.type);
	} while (lexer.currentChar);

	return 0;
}