#define _CRT_SECURE_NO_WARNINGS
#include "Lexer.h"
#include "LexerFSM.h"
#include "ErrorHandler.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void lexemeSizeExpander(Token *token, int *capacity) {
	char* temp;
    if ((*capacity & 0x40000000) != 0)
        reportError(ERROR_INTERNAL, "Maximum identifier name size reached.");

	(*capacity) <<= 1;
	temp = (char*)realloc(token->lexeme, *capacity * sizeof(char));

	if (!temp)
		exit(EXIT_FAILURE);

	token->lexeme = temp;
}

Lexer* initLexer(char* path) {
	Lexer* lexer = (Lexer*) malloc(sizeof(Lexer));
	if (!lexer)
        reportError(ERROR_INTERNAL, "Failed to allocate memory for Lexer.");

    assert(lexer);
	lexer->lexerFSM = initLexerFSM();
	lexer->fp = fopen(path, "r");

	lexer->indentStack[0] = 0;
	lexer->indentTop = 0;
	lexer->pendingDedents = 0;
	lexer->isStartOfLine = 1;

	return lexer;
}

Token* createToken(const char* lexeme, TokenType type) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (!token) reportError(ERROR_INTERNAL, "Failed to allocate new token");
    
    assert(token);
    token->type = type;
    token->lexeme = _strdup(lexeme);
    return token;
}

Token* handleIndentation(Lexer* lexer, char* c) {
    int currentIndent = 0, prevIndent;
    while (*c == ' ' || *c == '\t') {
        currentIndent += (*c == '\t') ? 4 : 1;
        *c = fgetc(lexer->fp);
    }

    if (*c == '\n' || *c == '\r')
        return NULL;

    prevIndent = lexer->indentStack[lexer->indentTop];

    if (currentIndent > prevIndent) {
        if (lexer->indentTop + 1 >= MAX_INDENT_DEPTH)
            reportError(ERROR_LEXICAL, "Max indentation depth reached!");

        lexer->indentStack[++lexer->indentTop] = currentIndent;
        lexer->isStartOfLine = 0;
        ungetc(*c, lexer->fp);
        return createToken("INDENT", TOKEN_INDENT);
    }

    if (currentIndent < prevIndent) {
        int dedents = 0;
        while (lexer->indentTop > 0 && lexer->indentStack[lexer->indentTop] > currentIndent) {
            lexer->indentTop--;
            dedents++;
        }

        if (lexer->indentStack[lexer->indentTop] != currentIndent)
            reportError(ERROR_LEXICAL, "Inconsistent indentation! Expected %d spaces but found %d.",
                lexer->indentStack[lexer->indentTop], currentIndent);

        lexer->isStartOfLine = 0;
        ungetc(*c, lexer->fp);
        lexer->pendingDedents = dedents - 1;
        return createToken("DEDENT", TOKEN_DEDENT);
    }

    lexer->isStartOfLine = 0;
    return NULL;
}

void handleNumberFormatting(Token* token, int* counter, char c) {
    if (*counter == 1 && token->lexeme[0] == '0' && c >= '0' && c <= '9')
        *counter = 0;
    else if (*counter == 2 && token->lexeme[0] == '-' && token->lexeme[1] == '0' && c >= '0' && c <= '9')
        *counter = 1;
}

Token* runFSM(Lexer* lexer, char c) {
    Token* token = (Token*)malloc(sizeof(Token));
    int capacity = LONGEST_WORD_LENGTH + 1;
    LexerFSM* fsm = lexer->lexerFSM;
    int counter = 0;
    
    if (!token)
        exit(EXIT_FAILURE);

    token->lexeme = (char*)malloc(capacity * sizeof(char));

    if (!token->lexeme)
        exit(EXIT_FAILURE);

    while (fsm->currentState != STATE_ACCEPT) {
        unsigned short prevState;
        if (getTokenType(fsm->transitionTable, fsm->currentState) == TOKEN_NUMBER)
            handleNumberFormatting(token, &counter, c);

        token->lexeme[counter++] = c;
        prevState = fsm->currentState;
        token->type = advance(fsm, c);

        counter -= (token->type != TOKEN_IDLE || fsm->currentState == STATE_COMMENT);

        if (token->type == TOKEN_IDLE && !fsm->currentState)
            counter = 0;

        if (counter >= capacity)
            lexemeSizeExpander(token, &capacity);

        if (token->type == TOKEN_IDLE)
            c = fgetc(lexer->fp);
    }

    ungetc(c, lexer->fp);

    if (token->type == TOKEN_NEWLINE)
        lexer->isStartOfLine = 1;

    token->lexeme[counter] = '\0';
    token->lexeme = (char*)realloc(token->lexeme, (counter + 1) * sizeof(char));

    return token;
}

Token* nextToken(Lexer* lexer) {
    LexerFSM* fsm = lexer->lexerFSM;
    char c;

    if (lexer->pendingDedents > 0) {
        lexer->pendingDedents--;
        return createToken("DEDENT", TOKEN_DEDENT);
    }

    c = fgetc(lexer->fp);

    while (lexer->isStartOfLine) {
        Token* indentToken = handleIndentation(lexer, &c);
        if (indentToken) return indentToken;
        if (lexer->isStartOfLine) c = fgetc(lexer->fp);
    }

    fsm->currentState = STATE_START;

    Token* token = runFSM(lexer, c);

    return token;
}

void freeLexer(Lexer* lexer) {
	freeLexerFSM(lexer->lexerFSM);
    fclose(lexer->fp);
	free(lexer);
}