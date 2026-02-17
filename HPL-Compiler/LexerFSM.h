#pragma once
#include "TransitionTable.h"

typedef struct {
	unsigned short currentState;
	TransitionTable* transitionTable;
} LexerFSM;


// Initializes the lexerFSM
LexerFSM* initLexerFSM();
// Advances one character in the FSM
void advance(LexerFSM* lexerFSM, char input);