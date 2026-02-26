#include "Compiler.h"
#include <stdlib.h>

// TEST FUNCTIONS
void testLexer(Compiler* compiler) {
	int currentBytes, numStates, numChars, matrixBytes;
	Token token;
	while ((token = nextToken(compiler->lexer)).type != TOKEN_EOF) {
		printf("Token: %s -- %d\n", token.lexeme, token.type);
		free(token.lexeme);
	}

	currentBytes = calculateTransitionTableMemory(compiler->lexer->lexerFSM->transitionTable);

	numStates = compiler->lexer->lexerFSM->transitionTable->stateCounter + 1;
	numChars = 71; // Letters + Digits + Symbols = 53 + 10 + 8 = 71
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
	ParserAction* action;
	Token token;

	while ((token = nextToken(compiler->lexer)).type != TOKEN_EOF) {
		action = getAction(parser->table, token, lookahed(parser->stack).state);

		if (action->type == SHIFT) {
			action->data.shift_item.token = token;
			shift(&parser->stack, action->data.shift_item);
		} else {
			reduce(&parser->stack, action->data.reduce_count);
		}

		printf("Parsed Token: %s Succesfuly!\n", token.lexeme);
	}
}

Compiler* initCompiler(char* filePath){
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
	testLexer(compiler);

	
}

void freeCompiler(Compiler* compiler) {
	freeLexer(compiler->lexer);
	freeParser(compiler->parser);

	free(compiler);
}