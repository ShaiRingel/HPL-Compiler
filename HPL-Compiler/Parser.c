#include "Parser.h"
#include <stdlib.h>
#include <stdio.h>

Parser* initParser() {
	Parser* parser = (Parser*)malloc(sizeof(Parser));
	if (!parser) {
		printf(RED "Error: Failed to allocate memory for Parser\n" RESET);
		exit(EXIT_FAILURE);
	}

	parser->table = initParsingTable();
	parser->stack = initStack();

	return parser;
}