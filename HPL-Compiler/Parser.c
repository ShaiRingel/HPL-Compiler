#include "Parser.h"
#include <stdlib.h>
#include <stdio.h>

void createTable(Parser* parser) {
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

	createTable(parser);

	return parser;
}