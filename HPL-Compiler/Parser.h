#pragma once
#include "FileReader.h"
#include "Lexer.h"
#include "LinkedList.h"
#include "HashMap.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum {
	false, true
} Bool;

typedef struct {
	Lexer lexer;
	HashMap symbolTable;
	Token currentToken;
	Token peekToken;
} Parser;

// Initialize the parser
void initParser(Parser *parser, FileDetails fileDetails);

// 
void program(Parser* parser, FileDetails filedetails);