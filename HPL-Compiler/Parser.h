#pragma once
#include "FileReader.h"
#include "Lexer.h"
#include "AST.h"

typedef enum {
	false, true
} Bool;

typedef struct {
	Lexer *lexer;
	Token currentToken;
	Token peekToken;
} Parser;

// Initialize the parser
Parser* initParser(FileDetails fileDetails);

// 
ASTNode* program(FileDetails fileDetails);