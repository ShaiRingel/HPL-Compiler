#include "Parser.h"

Bool checkToken(Parser* parser, TokenType type);

Bool checkPeek(Parser* parser, TokenType type);

void match(Parser* parser, TokenType type, const char* expectedName);

void nextToken(Parser* parser);

void initParser(Parser *parser, FileDetails fileDetails) {
	initLexer(&parser->lexer, fileDetails);
	initHashMap(&parser->symbolTable);
	nextToken(parser);
	nextToken(parser);
}


Bool checkToken(Parser* parser, TokenType type) {
	return parser->currentToken.type == type;
}


Bool checkPeek(Parser* parser, TokenType type) {
	return parser->peekToken.type == type;
}


void match(Parser* parser, TokenType type, const char* expectedName) {
    if (parser->currentToken.type != type) {
        fprintf(stderr, "Expected %s, got %s\n", expectedName, parser->currentToken.lexeme);
        exit(EXIT_FAILURE);
    }
    nextToken(parser);
}


void nextToken(Parser *parser) {
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

            Token ident = parser->currentToken;
            match(parser, TOKEN_IDENT, "identifier");
            match(parser, TOKEN_BE, "be");

            puts("BE-STATEMENT");

            listItemType item;
            item.dataType = parser->currentToken.lexeme;
            item.scope = "global";

            insert(&parser->symbolTable, ident.lexeme, item);

            nextToken(parser);
            break;

        case TOKEN_SET:
            puts("SET-STATEMENT");
            nextToken(parser);

            match(parser, TOKEN_IDENT, "Identifier");
            match(parser, TOKEN_TO, "to");

            puts("TO-STATEMENT");

            expression(parser);
            break;

        case TOKEN_INCREASE:
            puts("INCREASE-STATEMENT");
            nextToken(parser);

            match(parser, TOKEN_IDENT, "Identifier");
            match(parser, TOKEN_BY, "by");

            puts("BY-STATEMENT");

            expression(parser);
            break;

        case TOKEN_DECREASE:
            puts("DECREASE-STATEMENT");
            nextToken(parser);

            match(parser, TOKEN_IDENT, "Identifier");
            match(parser, TOKEN_BY, "by");

            puts("BY-STATEMENT");

            expression(parser);
            break;

        case TOKEN_MULTIPLY:
            puts("MULTIPLY-STATEMENT");
            nextToken(parser);

            match(parser, TOKEN_IDENT, "Identifier");
            match(parser, TOKEN_BY, "by");

            puts("BY-STATEMENT");

            expression(parser);
            break;

        case TOKEN_DIVIDE:
            puts("DIVIDE-STATEMENT");
            nextToken(parser);

            match(parser, TOKEN_IDENT, "Identifier");
            match(parser, TOKEN_BY, "by");

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
    listItemType currIdent;
    printf("PRIMARY (%s)\n", parser->currentToken.lexeme);

    switch (parser->currentToken.type) {
        case TOKEN_NUMBER:
            atoi(parser->currentToken.lexeme);
            break;

        case TOKEN_IDENT:
            currIdent = get(&parser->symbolTable, parser->currentToken.lexeme);

            if (currIdent.dataType) {
                printf("IDENTIFIER-FOUND (");
                printf("{ DataType: %s, Scope: %s })\n", currIdent.dataType, currIdent.scope);
            } else {
                fprintf(stderr, "Identifier not found: %s\n",
                    parser->currentToken.lexeme);
                exit(EXIT_FAILURE);
            }
            break;

        default:
            fprintf(stderr, "Unexpected token: %s\n",
                parser->currentToken.lexeme);
            exit(EXIT_FAILURE);
    }
    
    nextToken(parser);
}