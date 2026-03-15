#include "Parser.h"
#include "ErrorHandler.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

Parser* initParser() {
	Parser* parser = (Parser*)malloc(sizeof(Parser));
    ParsingStackItem item;

	if (!parser)
		reportError(ERROR_INTERNAL, "Failed to allocate memory for Parser");

    assert(parser);
	parser->table = initParsingTable();
	parser->stack = initParsingStack();
	parser->cst = createASTNode(0, 0, NULL);

    item.state = 0;
    item.token.type = TOKEN_EOF;
    item.token.lexeme = "START";
    shift(&(parser->stack), item);

	return parser;
}

void shiftAction(ParsingStack** stack, ParseAction action, Token* token) {
    ParsingStackItem item;

    item.token = *token;
    item.state = (unsigned short)action.value;
    item.astNode = createASTNode(TERMINAL, token->type, token);

    shift(stack, item);
}

CSTNode* reduceAction(ParsingStack** stack, ParseAction action, int* lhs) {
    int ruleLen = getRuleLength(action.value);
    *lhs = getLHS(action.value);

    CSTNode* subtree = reduce(stack, ruleLen, *lhs);

    return subtree;
}

void gotoAction(ParsingStack** stack, ParseAction action, int lhs, CSTNode* subtree) {
    ParsingStackItem item;

    item.token.type = (TokenType)lhs;
    item.token.lexeme = nonTerminalNames[lhs - NON_TERMINAL_PROG];
    item.state = (unsigned short)action.value;
    item.astNode = subtree;
    shift(stack, item);
}

int nextAction(Parser* parser, Token* token, int* next) {
    ParsingStackItem top;
    ParsingStack** stack;
    ParseAction action;
    CSTNode* subTree;
    int lhs;

    *next = 0;
    stack = &parser->stack;
    top = lookahed(*stack);
    action = getEntry(parser->table, top.state, token->type);

    switch (action.type) {
    case ACTION_SHIFT:
        shiftAction(stack, action, token);
        *next = 1;
        break;

    case ACTION_REDUCE:
        subTree = reduceAction(stack, action, &lhs);

        top = lookahed(*stack);
        action = getEntry(parser->table, top.state, lhs);

        if (action.type == ACTION_GOTO)
            gotoAction(stack, action, lhs, subTree);
        else {
            reportError(ERROR_SYNTAX, "Invalid GOTO action in parser");
        }

        break;
        
    case ACTION_ACCEPT:
        printf(GREEN "--- SUCCESS: ACCEPT ---\n" RESET);
        free(parser->cst);
        parser->cst = (*stack)->value.astNode;
        return 1;

    case ACTION_ERROR:
    default:
        reportError(ERROR_SYNTAX, "Syntax error encountered during parsing");
    }

    return 0;
}

void freeParser(Parser* parser) {
	freeParsingTable(parser->table);
	freeParsingStack(parser->stack);
    freeAST(parser->cst);
	free(parser);
}