#include "Parser.h"
#include <stdlib.h>
#include <stdio.h>

void createTable(ParsingTable* table) {
	// Create all actions
	

	// Create all gotos
	
}

Parser* initParser() {
	Parser* parser = (Parser*)malloc(sizeof(Parser));
	if (!parser) {
		printf(RED "Error: Failed to allocate memory for Parser\n" RESET);
		exit(EXIT_FAILURE);
	}

	parser->table = initParsingTable();
	parser->stack = initParsingStack();

	ParsingStackItem item = { 0 };
	shift(&parser->stack, item);

	createTable(parser->table);

	return parser;
}

void freeParser(Parser* parser) {
	freeParsingTable(parser->table);
	freeParsingStack(parser->stack);
	free(parser);
}