#include "Compiler.h"
#include <stdlib.h>

// TEST FUNCTIONS
void testLexer(Compiler* compiler) {
	int currentBytes, numStates, numChars, matrixBytes;
	Token* token;

	while ((token = nextToken(compiler->lexer))->type != TOKEN_EOF) {
		if (token->type != TOKEN_NEWLINE) {
			printf("Token: %s -- %d\n", token->lexeme, token->type);
			free(token->lexeme);
			free(token);
		}
	}

	currentBytes = calculateTransitionTableMemory(compiler->lexer->lexerFSM->transitionTable);

	numStates = compiler->lexer->lexerFSM->transitionTable->stateCounter + 1;
	numChars = 72; // Letters + Digits + Symbols = 53 + 10 + 8 = 71
	matrixBytes = numStates * numChars * sizeof(unsigned short);

	printf("\n--- Transition table implementation memory Comparison ---\n");
	printf("Usual (Adjacency Matrix):\t%d bytes (%.2f KB)\n",
		matrixBytes, matrixBytes / 1024.0);
	printf("Current (Nested HashMaps):\t%d bytes (%.2f KB)\n",
		currentBytes, currentBytes / 1024.0);
	printf("Memory Saved:\t\t\t%d bytes (%.2f KB) -- %.2f%%\n",
		matrixBytes - currentBytes, (matrixBytes - currentBytes) / 1024.0,
		100.0 - 100.0 * currentBytes / matrixBytes);
}

void testParser(Compiler* compiler) {
    Parser* parser = compiler->parser;
    Token* currentToken = NULL;
	int stage, next;

	stage = next = 1;

    printf("--- Starting Parser Test: ---\n\n");
	do {
		if (next)
			currentToken = nextToken(compiler->lexer);

		if (currentToken->type == TOKEN_NEWLINE)
			currentToken = nextToken(compiler->lexer);

		printf("%03d Stack: ", stage++);
		printStack(parser->stack);
	} while (!nextAction(parser, currentToken, &next));

	printAST(parser->ast);
}

Compiler* initCompiler(char* filePath) {
	Compiler* compiler = (Compiler*) malloc(sizeof(Compiler));

	if (!compiler) {
		printf(RED "Error: Failed to allocate memory for compiler\n" RESET);
		exit(EXIT_FAILURE);
	}

	compiler->lexer = initLexer(filePath);
	compiler->parser = initParser();
	compiler->analyzer = initSemanticAnalyzer();
	
	return compiler;
}

void startCompiler(Compiler* compiler) {
	 // testLexer(compiler);
	 testParser(compiler);
}

void freeCompiler(Compiler* compiler) {
	freeLexer(compiler->lexer);
	freeParser(compiler->parser);
	freeSemanticAnalyzer(compiler->analyzer);

	free(compiler);
}