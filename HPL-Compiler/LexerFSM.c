#include "LexerFSM.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define isDelmiter(c) isspace((c)) || (c) == '.' || (c) == EOF

size_t calculateTransitionTableMemory(const TransitionTable* table) {
    size_t total = 0;

    // --- TransitionTable struct + bucket array ---
    total += sizeof(TransitionTable);
    total += table->capacity * sizeof(StateBucket*);

    // --- Iterate through all state buckets ---
    for (int i = 0; i < table->capacity; i++) {
        StateBucket* sb = table->buckets[i];

        while (sb) {
            // StateBucket struct
            total += sizeof(StateBucket);

            // CharMap struct + bucket array
            if (sb->value) {
                total += sizeof(CharMap);
                total += sb->value->capacity * sizeof(CharBucket*);

                // Count CharBuckets (transitions)
                for (int j = 0; j < sb->value->capacity; j++) {
                    CharBucket* cb = sb->value->buckets[j];
                    while (cb) {
                        total += sizeof(CharBucket);
                        cb = cb->next;
                    }
                }
            }

            sb = sb->next;
        }
    }

    return total;
}

void addKeyword(TransitionTable* table, const char* word, TokenType token) {
    int i, nextId, current;

    current = 0;
    for (i = 0; word[i] != '\0'; i++) {
        nextId = getState(table, (unsigned short)current, word[i]);
        if (!nextId) {
            nextId = ++table->stateCounter;
            insertTransition(table, (unsigned short)current, word[i], (unsigned short)nextId);
        }
        current = nextId;
    }
    setToken(table, (unsigned short)current, token);
}

void buildTransitionTable(TransitionTable* table) {
    int i;

    // WHITESPACES
    insertTransition(table, 0, ' ', ++table->stateCounter);
    insertTransition(table, 0, '\n', table->stateCounter);
    insertTransition(table, 0, '\t', table->stateCounter);
    insertTransition(table, 0, '\r', table->stateCounter);
    insertTransition(table, 0, '\v', table->stateCounter); 
    insertTransition(table, 0, '\f', table->stateCounter);
    insertTransition(table, table->stateCounter, ' ', table->stateCounter);
    insertTransition(table, table->stateCounter, '\n', table->stateCounter);
    insertTransition(table, table->stateCounter, '\t', table->stateCounter);
    insertTransition(table, table->stateCounter, '\r', table->stateCounter);
    insertTransition(table, table->stateCounter, '\v', table->stateCounter);
    insertTransition(table, table->stateCounter, '\f', table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_IDLE);

    setToken(table, table->stateCounter, TOKEN_IDENT);

    // KEYWORDS
    for (i = 0; i < sizeof(keywordTable) / sizeof(keywordTable[0]); i++)
        addKeyword(table, keywordTable[i].keyword, keywordTable[i].type);

    // NUMBERS
    ++table->stateCounter;
    for (i = '0'; i <= '9'; i++) {
        insertTransition(table, 0, i, table->stateCounter);
        insertTransition(table, table->stateCounter, i, table->stateCounter);
    }
    setToken(table, table->stateCounter, TOKEN_NUMBER);

    // SPECIAL CHARACTERS
    insertTransition(table, 0, '(', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_LPAREN);
    insertTransition(table, 0, ')', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_RPAREN);
    insertTransition(table, 0, ':', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_COLON);
    insertTransition(table, 0, '+', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_PLUS);
    insertTransition(table, 0, '.', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_EOS);
    insertTransition(table, 0, EOF, ++table->stateCounter);
    setToken(table, table->stateCounter++, TOKEN_EOF);
}

LexerFSM* initLexerFSM() {
    LexerFSM* fsm;

    fsm = (LexerFSM*)malloc(sizeof(LexerFSM));
    if (!fsm) exit(EXIT_FAILURE);

    fsm->currentState = 0;
    fsm->transitionTable = initTransitionTable();
    buildTransitionTable(fsm->transitionTable);

    return fsm;
}

TokenType advance(LexerFSM* lexerFSM, char input) {
    TokenType token;
    int nextS;

    if (lexerFSM->currentState == 1 && !isspace(input))
        lexerFSM->currentState = 0;

    nextS = getState(lexerFSM->transitionTable, lexerFSM->currentState, input);

    if (nextS != 0) {
        lexerFSM->currentState = (unsigned short)nextS;
        return 0;
    }
    
    token = getTokenType(lexerFSM->transitionTable, lexerFSM->currentState);
    lexerFSM->currentState = 0;

    if (!token) {
        if (!isspace(input) && input != '.') {
            lexerFSM->currentState = lexerFSM->transitionTable->stateCounter;
            token = TOKEN_IDLE;
        }

        token = TOKEN_IDENT;
    }

    return isDelmiter(input) ? token : TOKEN_IDLE;
}