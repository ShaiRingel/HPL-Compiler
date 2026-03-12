#pragma once
#include "ParsingDefinitions.h"
#include "ParsingStack.h"
#include "ParsingTable.h"
#include "CST.h"
#include <string.h>

typedef struct {
	ParsingTable* table;
	ParsingStack* stack;
	CSTNode* ast;
} Parser;

// Initialize parsers components
Parser* initParser();

// 
int nextAction(Parser* parser, Token* token, int* cont);

// Frees the entire Parser and all associated memory
void freeParser(Parser* parser);