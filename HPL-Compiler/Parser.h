#pragma once
#include "ParsingStack.h"
#include "ParsingTable.h"

typedef struct {
	ParsingTable* table;
	ParsingStack* stack;
} Parser;

// Initialize parsers components
Parser* initParser();

// Frees the entire Parser and all associated memory
void freeParser(Parser* parser);