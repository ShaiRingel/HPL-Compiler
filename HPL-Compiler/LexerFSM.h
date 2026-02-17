#pragma once
#include "TransitionTable.h"

const char* languageKeyWords[] = {
    "add",
    "and",
    "ask",
    "atLeast",
    "atMost",
    "atPosition",
    "be",
    "by",
    "decimal",
    "divide",
    "dividedBy",
    "equalsTo",
    "greaterThan",
    "in",
    "integer",
    "lessThan",
    "not",
    "notEqualsTo",
    "or",
    "remainderOf",
    "storeInto",
    "subtract",
    "times",
    "then",
    "text",
    "to",
    "with",
    "Decrease",
    "Divide",
    "Foreach",
    "Get",
    "If",
    "Increase",
    "Let",
    "Multiply",
    "Otherwise",
    "Repeat",
    "Return",
    "Set",
    "Show",
    "To",
    "While"
};

typedef struct {
	unsigned short currentState;
	TransitionTable* transitionTable;
} LexerFSM;


// Initializes the lexerFSM
LexerFSM* initLexerFSM();
// Advances one character in the FSM
void advance(LexerFSM* lexerFSM, char input);