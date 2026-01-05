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
void expression(Parser *parser);
void term(Parser *parser);
void factor(Parser *parser);

void program(Parser *parser, FileDetails filedetails) {
	puts("Initializing FileReader, Lexer and parser!\n");

	initParser(parser, filedetails);

	puts("Filereader, lexer and parser initialized succefully!\n");

	puts("Starting program!\n");

	while (!checkToken(parser, TOKEN_EOF))
		statement(parser);
}

void statement(Parser *parser) {
    switch (parser->currentToken.type) {
        case TOKEN_SHOW:
            puts("SHOW-STATEMENT");
            nextToken(parser);
            nextToken(parser);
            break;

        case TOKEN_LET:
            puts("LET-STATEMENT");
            nextToken(parser);

            match(parser, (Token) { TOKEN_IDENT, "Identifier" });
            match(parser, (Token) { TOKEN_BE, "be" });

            puts("BE-STATEMENT");

            nextToken(parser);
            break;

        case TOKEN_SET:
            puts("SET-STATEMENT");
            nextToken(parser);

            match(parser, (Token) { TOKEN_IDENT, "Identifier" });
            match(parser, (Token) { TOKEN_TO, "to" });

            puts("TO-STATEMENT");

            expression(parser);
            break;

        case TOKEN_INCREASE:
            puts("INCREASE-STATEMENT");
            nextToken(parser);

            match(parser, (Token) { TOKEN_IDENT, "Identifier" });
            match(parser, (Token) { TOKEN_BY, "by" });

            puts("BY-STATEMENT");

            expression(parser);
            break;

        case TOKEN_DECREASE:
            puts("DECREASE-STATEMENT");
            nextToken(parser);

            match(parser, (Token) { TOKEN_IDENT, "Identifier" });
            match(parser, (Token) { TOKEN_BY, "by" });

            puts("BY-STATEMENT");

            expression(parser);
            break;

        case TOKEN_MULTIPLY:
            puts("MULTIPLY-STATEMENT");
            nextToken(parser);

            match(parser, (Token) { TOKEN_IDENT, "Identifier" });
            match(parser, (Token) { TOKEN_BY, "by" });

            puts("BY-STATEMENT");

            expression(parser);
            break;

        case TOKEN_DIVIDE:
            puts("DIVIDE-STATEMENT");
            nextToken(parser);

            match(parser, (Token) { TOKEN_IDENT, "Identifier" });
            match(parser, (Token) { TOKEN_BY, "by" });

            puts("BY-STATEMENT");

            expression(parser);
            break;

        case TOKEN_EOF:
            return;

        default:
            fprintf(stderr, "Unexpected token: %s\n",
                parser->currentToken.lexeme);
            exit(EXIT_FAILURE);
    }

    puts("");
}

void expression(Parser *parser) {
    printf("EXPRESSION\n");

    term(parser);
    while (checkToken(parser, TOKEN_ADD) ||
        checkToken(parser, TOKEN_SUB)){
        nextToken(parser);
        term(parser);
    }
}

void term(Parser* parser) {
    printf("TERM\n");
    
    factor(parser);
    while (checkToken(parser, TOKEN_MUL) ||
        checkToken(parser, TOKEN_DIV)) {
        nextToken(parser);
        factor(parser);
    }
}


void factor(Parser* parser) {
    printf("PRIMARY (%s)\n", parser->currentToken.lexeme);

    switch (parser->currentToken.type) {
        case TOKEN_NUMBER:
            // atoi(parser->currentToken.lexeme);
            break;
        case TOKEN_IDENT:
            // TODO: Check if symbol exists already once symbol table is available
            break;

        default:
            fprintf(stderr, "Unexpected token: %s\n",
                parser->currentToken.lexeme);
            exit(EXIT_FAILURE);
    }

    nextToken(parser);
}