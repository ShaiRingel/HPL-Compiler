#include "Global.h"
#include "LexerFSM.h"
#include <stdio.h>
#include <stdlib.h>

void buildTransitionTable(TransitionTable* table) {
	const char *p;
	int state, i;

	for (i = 0; i < sizeof(languageKeyWords) / sizeof(languageKeyWords[0]); i++)
		for (state = 0, p = languageKeyWords[i]; p;
			p++, state = getState(table, state, *p))
			insertTransition(table, state, *p);
}

LexerFSM* initLexerFSM() {
	LexerFSM* lexerFSM = (LexerFSM*) malloc(sizeof(LexerFSM));
	if (!lexerFSM) {
		printf(RED "Error: Failed to allocate memory for lexerFSM\n" RESET);
		exit(EXIT_FAILURE);
	}

	lexerFSM->currentState = 0;
	lexerFSM->transitionTable = initTransitionTable();
	buildTransitionTable(lexerFSM->transitionTable);

	return lexerFSM;
}

void advance(LexerFSM* lexerFSM, char input) {
	
}