#define _CRT_SECURE_NO_WARNINGS
#include "Lexer.h"
#include "LexerFSM.h"
#include <stdlib.h>

void lexemeSizeExpander(Token *token, int *capacity) {
	char* temp;
	if (*capacity & 8000 != 0) {
		printf(RED "Error: Maximum identifier name size reached!\n" RESET);
		exit(EXIT_FAILURE);
	}

	(*capacity) <<= 1;
	temp = (char*)realloc(token->lexeme, *capacity * sizeof(char));

	if (!temp) {
		exit(EXIT_FAILURE);
	}

	token->lexeme = temp;
}

Lexer* initLexer(char* path) {
	Lexer* lexer = (Lexer*) malloc(sizeof(Lexer));
	if (!lexer) {
		printf(RED "Error: Failed to allocate memory for lexer\n" RESET);
		exit(EXIT_FAILURE);
	}

	lexer->lexerFSM = initLexerFSM();
	lexer->fp = fopen(path, "r");

	lexer->indentStack[0] = 0;
	lexer->indentTop = 0;
	lexer->pendingDedents = 0;
	lexer->isStartOfLine = 1;

	return lexer;
}

Token* nextToken(Lexer* lexer) {
    LexerFSM* fsm = lexer->lexerFSM;
    unsigned short* curr = &fsm->currentState;
    int counter, capacity;
    char c;
    Token* token;

    token = (Token*)malloc(sizeof(Token));
    if (!token) exit(EXIT_FAILURE);

    capacity = LONGEST_WORD_LENGTH + 1;
    token->type = TOKEN_IDLE;
    token->lexeme = (char*)malloc(capacity * sizeof(char));
    if (!token->lexeme) exit(EXIT_FAILURE);

    if (lexer->pendingDedents > 0) {
        lexer->pendingDedents--;
        token->type = TOKEN_DEDENT;
        strcpy(token->lexeme, "DEDENT");
        return token;
    }

    c = fgetc(lexer->fp);

    while (lexer->isStartOfLine) {
        int currentIndent = 0;

        while (c == ' ' || c == '\t') {
            currentIndent += (c == '\t') ? 4 : 1;
            c = fgetc(lexer->fp);
        }

        if (c == '\n' || c == '\r') {
            c = fgetc(lexer->fp);
            continue;
        }
        else if (c == EOF) {
            break;
        }

        int prevIndent = lexer->indentStack[lexer->indentTop];

        if (currentIndent > prevIndent) {
            if (lexer->indentTop + 1 >= MAX_INDENT_DEPTH) {
                printf(RED "Error: Max indentation depth reached!\n" RESET);
                exit(EXIT_FAILURE);
            }
            lexer->indentStack[++lexer->indentTop] = currentIndent;
            lexer->isStartOfLine = 0;
            ungetc(c, lexer->fp);

            token->type = TOKEN_INDENT;
            strcpy(token->lexeme, "INDENT");
            return token;

        }
        else if (currentIndent < prevIndent) {
            int dedents = 0;
            while (lexer->indentTop > 0 && lexer->indentStack[lexer->indentTop] > currentIndent) {
                lexer->indentTop--;
                dedents++;
            }

            if (lexer->indentStack[lexer->indentTop] != currentIndent) {
                printf(RED "Error: Inconsistent indentation!\n" RESET);
                exit(EXIT_FAILURE);
            }

            lexer->isStartOfLine = 0;
            ungetc(c, lexer->fp);

            if (dedents > 0) {
                lexer->pendingDedents = dedents - 1;
                token->type = TOKEN_DEDENT;
                strcpy(token->lexeme, "DEDENT");
                return token;
            }
        }
        else {
            lexer->isStartOfLine = 0;
            break;
        }
    }

    if (c == EOF) {
        if (lexer->indentTop > 0) {
            lexer->indentTop--;
            token->type = TOKEN_DEDENT;
            strcpy(token->lexeme, "DEDENT");
            return token;
        }
        token->type = TOKEN_EOF;
        strcpy(token->lexeme, "EOF");
        return token;
    }

    fsm->currentState = 0;
    counter = 0;

    while (*curr != STATE_ACCEPT) {
        unsigned short prevState;

        if (getTokenType(fsm->transitionTable, *curr) == TOKEN_NUMBER) {
            if (counter == 1 && token->lexeme[0] == '0' && c >= '0' && c <= '9')
                counter = 0;

            else if (counter == 2 && token->lexeme[0] == '-' && token->lexeme[1] == '0' && c >= '0' && c <= '9')
                counter = 1;
        }
        
        token->lexeme[counter++] = c;
        prevState = *curr;
        token->type = advance(fsm, c);

        if (prevState == STATE_COMMENT && *curr == 0 && c == '\n') {
            token->type = TOKEN_NEWLINE;
            lexer->isStartOfLine = 1;
            c = fgetc(lexer->fp);
            break;
        }

        counter -= (token->type != TOKEN_IDLE || *curr == STATE_COMMENT);

        if (token->type == TOKEN_IDLE && !*curr)
            counter = 0;

        if (counter >= capacity)
            lexemeSizeExpander(token, &capacity);

        if (token->type == TOKEN_IDLE)
            c = fgetc(lexer->fp);
    }

    ungetc(c, lexer->fp);

    if (token->type == TOKEN_NEWLINE) {
        lexer->isStartOfLine = 1;
    }

    token->lexeme[counter] = 0;
    token->lexeme = (char*)realloc(token->lexeme, (counter + 1) * sizeof(char));

    return token;
}

void freeLexer(Lexer* lexer) {
	freeLexerFSM(lexer->lexerFSM);
	free(lexer);
}