#ifndef LEXER_H
#define LEXER_H
#include "FileReader.h"

typedef enum {
	TOKEN_EOF = -1,
	TOKEN_NEWLINE,
	TOKEN_NUMBER,
	TOKEN_IDENT,
	TOKEN_STRING,
	// Keywords
	TOKEN_LET = 101,
	TOKEN_BE,
	TOKEN_SET,
	TOKEN_TO,
	TOKEN_BY,
	TOKEN_SHOW,
	TOKEN_INTEGER,
	TOKEN_INCREASE,
	TOKEN_DECREASE,
	// Operators
	TOKEN_EQ = 201,
	TOKEN_ADD,
	TOKEN_SUB,
	TOKEN_MUL,
	TOKEN_DIV,
	TOKEN_EQEQ,
	TOKEN_NOTEQ,
	TOKEN_LT,
	TOKEN_LTEQ,
	TOKEN_GT,
	TOKEN_GTEQ,
	// Special Symbols
	TOKEN_LPAREN = 301,
	TOKEN_RPAREN
} TokenType;

typedef struct {
	TokenType type;
	char* lexeme;
} Token;

typedef struct {
	FileDetails fileDetails;
	char currentChar;
	int currentRow;
	int currentCol;
} Lexer;

// Initialize the lexer
void initLexer(Lexer* lexer, FileDetails fileDetails);
// Read the next non-WhiteSpace character
void nextChar(Lexer* lexer);
// Skip the WhiteSpace characters
void skipWhiteSpaces(Lexer *lexer);
// Skip the Comments in the code
void skipComments(Lexer* lexer);
// Collects a full lexeme from the lexer until the condition returns false
char* getFullLexeme(Lexer* lexer, int (*condition)(int c));
// Turn character / string into corresponding token
Token tokenize(Lexer *lexer);

int numberCondition(int c);
int wordCondition(int c);

#endif