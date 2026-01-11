#include "Parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



Bool checkToken(Parser* parser, TokenType type);
Bool checkPeek(Parser* parser, TokenType type);
void match(Parser* parser, TokenType type, const char* expectedName);
void nextToken(Parser* parser);

Parser* initParser(FileDetails fileDetails) {
	Parser* parser = (Parser*)malloc(sizeof(Parser));

	if (!parser) {
		perror("Malloc failed!");
		exit(EXIT_FAILURE);
	}

	parser->lexer = initLexer(fileDetails);
	nextToken(parser);
	nextToken(parser);

	return parser;
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

void nextToken(Parser* parser) {
	parser->currentToken = parser->peekToken;
	parser->peekToken = tokenize(parser->lexer);
}

// production rules

ASTNode* statement(Parser* parser);
ASTNode* expression(Parser* parser);
ASTNode* term(Parser* parser);
ASTNode* factor(Parser* parser);

ASTNode* program(FileDetails filedetails) {
	puts("Initializing FileReader, Lexer and parser!\n");
	
	Parser* parser = initParser(filedetails);

	puts("Filereader, lexer and parser initialized succefully!\n");
	puts("Starting program!\n");

	ASTNode* root = createNode(NODE_PROGRAM);
	ASTNode* tail = root;

	while (!checkToken(parser, EOF)) {
		tail->next = statement(parser);
		tail = tail->next;
	}

	return root;
}

ASTNode* statement(Parser* parser) {
	TokenType type = parser->currentToken.type;
	ASTNode* node = NULL;
	char* ident;

	switch (type) {
		case TOKEN_SHOW:
			node = createNode(NODE_SHOW);
			nextToken(parser);
			
			node->right = expression(parser);
			break;
		case TOKEN_ASK:

			break;
		case TOKEN_LET:
			nextToken(parser);

			if (!checkToken(parser, TOKEN_IDENT)) {
				fprintf(stderr, "Expected identifier after 'Let', got %s\n",
					parser->currentToken.lexeme);
				exit(EXIT_FAILURE);
			}

			node = createVariableDeclarationNode(parser->currentToken.lexeme);
			nextToken(parser);

			match(parser, TOKEN_BE, "be");

			if (checkToken(parser, TOKEN_INTEGER)) {
				node->right = createNode(NODE_TYPE_INT);
				nextToken(parser);
			}
			else if (checkToken(parser, TOKEN_REAL)) {
				node->right = createNode(NODE_TYPE_REAL);
				nextToken(parser);
			}
			else {
				fprintf(stderr, "Expected 'integer' or 'real' after 'be', got %s\n",
					parser->currentToken.lexeme);
				exit(EXIT_FAILURE);
			}

			break;
		case TOKEN_SET:
			nextToken(parser);

			if (!checkToken(parser, TOKEN_IDENT)) {
				fprintf(stderr, "Expected identifier after 'Let', got %s\n",
					parser->currentToken.lexeme);
				exit(EXIT_FAILURE);
			}
			ident = parser->currentToken.lexeme;
			nextToken(parser);

			match(parser, TOKEN_TO, "to");
			
			node = createAssignmentNode(ident, expression(parser));

			break;
		case TOKEN_INCREASE:
		case TOKEN_DECREASE:
		case TOKEN_MULTIPLY:
		case TOKEN_DIVIDE:
			nextToken(parser);

			if (!checkToken(parser, TOKEN_IDENT)) {
				fprintf(stderr, "Expected identifier after 'Let', got %s\n",
					parser->currentToken.lexeme);
				exit(EXIT_FAILURE);
			}
			ident = parser->currentToken.lexeme;
			nextToken(parser);

			match(parser, TOKEN_BY, "by");

			OpType opType = type - TOKEN_INCREASE + OP_ADD;
			node = createAssignmentNode(ident, 
				createBinOpNode(opType, createIdNode(ident), expression(parser)));
			
			break;
		case TOKEN_IF:

			break;
		case TOKEN_WHILE:

			break;
		case TOKEN_REPEAT:

			break;
		case TOKEN_FOREACH:

			break;
		case TOKEN_TOFUNC:

			break;
		default:
			fprintf(stderr, "Unexpected token: %s\n",
				parser->currentToken.lexeme);
			exit(EXIT_FAILURE);

	}
	
	return node;
}

ASTNode* expression(Parser* parser) {
	ASTNode* root = term(parser);
	TokenType tokenType = parser->currentToken.type;

	while (tokenType == TOKEN_ADD
		|| tokenType == TOKEN_SUB) {
		nextToken(parser);

		OpType opType = tokenType - TOKEN_ADD + OP_ADD;
		root = createBinOpNode(opType, root, term(parser));
		tokenType = parser->currentToken.type;
	}

	return root;
}

ASTNode* term(Parser* parser) {
	ASTNode* root = factor(parser);
	TokenType tokenType = parser->currentToken.type;

	while (tokenType == TOKEN_MUL
		|| tokenType == TOKEN_DIV) {
		nextToken(parser);

		OpType opType = tokenType - TOKEN_MUL + OP_MUL;
		root = createBinOpNode(opType, root, factor(parser));
		tokenType = parser->currentToken.type;
	}

	return root;
}

ASTNode* factor(Parser* parser) {
	ASTNode* node = NULL;

	switch (parser->currentToken.type) {
		case TOKEN_NUMBER:
			node = createLiteralNode(atoi(parser->currentToken.lexeme));
			break;
		case TOKEN_IDENT:
			node = createIdNode(parser->currentToken.lexeme);
			break;
		default:
			fprintf(stderr, "Unexpected token: %s\n",
				parser->currentToken.lexeme);
			exit(EXIT_FAILURE);
	}

	nextToken(parser);
	return node;
}