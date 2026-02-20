#include "Compiler.h"
#include <stdlib.h>

Compiler* initCompiler(char* filePath){
	Compiler* compiler = (Compiler*) malloc(sizeof(Compiler));

	if (!compiler) {
		printf(RED "Error: Failed to allocate memory for compiler\n" RESET);
		exit(EXIT_FAILURE);
	}

	compiler->lexer = initLexer(filePath);

	return compiler;
}

Compiler* startCompiler(Compiler* compiler) {
	Token token;
	while ((token = nextToken(compiler->lexer)).type != TOKEN_EOF)
		printf("Token: %s -- %d\n", token.lexeme, token.type);
}