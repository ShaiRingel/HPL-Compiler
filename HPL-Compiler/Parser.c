#include "Parser.h"

void initParser(Parser *parser, FileDetails fileDetails) {
	initLexer(&parser->lexer, fileDetails);
	nextToken(parser);
	nextToken(parser);
}


Bool checkToken(Parser* parser, TokenType type) {
	return parser->currentToken.type == type;
}


Bool checkPeek(Parser* parser, TokenType type) {
	return parser->peekToken.type == type;
}


void match(Parser* parser, Token token) {
	if (!checkToken(parser, token.type)) {
		fprintf(stderr, "Expected %s, got %s\n", token.lexeme, parser->currentToken.lexeme);
		exit(EXIT_FAILURE);
	}
	
	nextToken(parser);
}


Token nextToken(Parser *parser) {
	parser->currentToken = parser->peekToken;
	parser->peekToken = tokenize(&parser->lexer);
}

// production rules

void statement(Parser *parser);

void program(Parser *parser, FileDetails filedetails) {
	puts("Initializing FileReader, Lexer and parser!\n");

	initParser(parser, filedetails);

	puts("Filereader, lexer and parser initialized succefully!\n");

	puts("Starting program!\n");

	while (!checkToken(parser, TOKEN_EOF))
		statement(parser);
}

void statement(Parser *parser) {

	if (checkToken(parser, TOKEN_SHOW)) {
		puts("SHOW-STATEMENT");
		nextToken(parser);
	}
	else if (checkToken(parser, TOKEN_LET)) {
		puts("LET-STATEMENT");
		nextToken(parser);
		Token tokenIdent = { TOKEN_IDENT, "Identifier" };
		Token tokenBe = { TOKEN_BE, "Be" };
		match(parser, tokenIdent);
		match(parser, tokenBe);
		puts("NICE!");
		nextToken(parser);
	}

	nextToken(parser);
}