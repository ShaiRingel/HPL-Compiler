#pragma once
#include "TransitionTable.h"
#include "Global.h"
#include <ctype.h>
#include <stdio.h>

#define isDelimiter(c) ( ((c) == EOF) || isspace((unsigned char)(c)) || (c) == '.' )

typedef struct {
    const char* keyword;
    TokenType type;
} KeywordEntry;

static KeywordEntry keywordTable[] = {
    // Variable declarations
    {"Let", TOKEN_LET},
    {"be", TOKEN_BE},
    {"number", TOKEN_INTEGER},
    {"text", TOKEN_TEXT},

    // Assignments
    {"Set", TOKEN_SET},
    {"to", TOKEN_TO},
    {"Increase", TOKEN_INCREASE},
    {"Decrease", TOKEN_DECREASE},
    {"Multiply", TOKEN_MULTIPLY},
    {"Divide", TOKEN_DIVIDE},
    {"by", TOKEN_BY},

    // Output
    {"Show", TOKEN_SHOW},

    // Conditionals
    {"If", TOKEN_IF},
    {"then", TOKEN_THEN},
    {"Otherwise", TOKEN_OTHERWISE},

    // Loops
    {"Repeat", TOKEN_REPEAT},
    {"iterations", TOKEN_ITERATIONS},
    {"Foreach", TOKEN_FOREACH},
    {"in", TOKEN_IN},
    {"While", TOKEN_WHILE},

    // Input
    {"Get", TOKEN_GET},
    {"ask", TOKEN_ASK},
    {"storeInto", TOKEN_STOREINTO},

    // Functions
    {"To", TOKEN_TOFUNC},
    {"with", TOKEN_WITH},
    {"Return", TOKEN_RETURN},

    // Operators (word forms)
    {"equalsTo", TOKEN_EQTO},
    {"notEqualsTo", TOKEN_NOTEQ},
    {"greaterThan", TOKEN_GT},
    {"lessThan", TOKEN_LT},
    {"atLeast", TOKEN_ATLEAST},
    {"atMost", TOKEN_ATMOST},
    {"remainderOf", TOKEN_REMAINDEROF},
    {"dividedBy", TOKEN_DIVIDEDBY},
    {"plus", TOKEN_ADD},
    {"minus", TOKEN_SUB},
    {"times", TOKEN_MUL},
    {"divide", TOKEN_DIV},
    {"and", TOKEN_AND},
    {"or", TOKEN_OR},
    {"not", TOKEN_NOT},

    // Arrays
    {"atPosition", TOKEN_ATPOSITION},
};

typedef struct {
	unsigned short currentState;
	TransitionTable* transitionTable;
} LexerFSM;

// Initializes and allocates a new LexerFSM
LexerFSM* initLexerFSM();

// Advances a single character in the FSM
TokenType advance(LexerFSM* lexerFSM, char input);

// Calculates the ammount of bytes the FSM occupies
int calculateTransitionTableMemory(const TransitionTable* table);

// Frees the entire LexerFSM and all associated memory
void freeLexerFSM(LexerFSM* lexerFSM);