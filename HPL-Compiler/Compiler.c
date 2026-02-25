#include "Compiler.h"
#include <stdlib.h>

// TEST FUNCTIONS
void testLexer(Compiler* compiler) {
	Token token;
	while ((token = nextToken(compiler->lexer)).type != TOKEN_EOF) {
		printf("Token: %s -- %d\n", token.lexeme, token.type);
		free(token.lexeme);
	}

	size_t currentBytes = calculateTransitionTableMemory(compiler->lexer->lexerFSM->transitionTable);

	int numStates = compiler->lexer->lexerFSM->transitionTable->stateCounter + 1;
	int numChars = 256;
	size_t matrixBytes = (size_t)numStates * numChars * sizeof(unsigned short);

	printf("\n--- Memory Comparison ---\n");
	printf("Adjacency Matrix:\t%zu bytes (%.2f KB)\n",
		matrixBytes, matrixBytes / 1024.0);
	printf("Current (Linked-List):\t%zu bytes (%.2f KB)\n",
		currentBytes, currentBytes / 1024.0);
	printf("Memory Saved:\t\t%zu bytes (%.2f KB) -- %.2f%%\n",
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