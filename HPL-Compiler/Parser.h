#pragma once
#include <stdlib.h>
#include <stdio.h>
#include "FileReader.h"
#include "Lexer.h"

typedef enum {
	false, true
} Bool;

typedef struct {
	Lexer lexer;
	Token currentToken;
	Token peekToken;
} Parser;

// Initialize the parser
void initParser(Parser *parser, FileDetails fileDetails);
// 
Bool checkToken(Parser *parser, Token token);
// 
Bool checkPeek(Parser *parser);
// 
void match(Parser *parser);
// 
Token nextToken(Parser *parser);

// production rules

void program(Parser* parser, FileDetails filedetails);