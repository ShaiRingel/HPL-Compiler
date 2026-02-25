#include "LexerFSM.h"
#include <stdlib.h>
#include <stdio.h>

int calculateTransitionTableMemory(const TransitionTable* table) {
    int i, j, total = 0;
    StateBucket* sb;
    CharBucket* cb;

    total += sizeof(TransitionTable);
    total += table->capacity * sizeof(StateBucket*);

    for (i = 0; i < table->capacity; i++) {
        sb = table->buckets[i];

        while (sb) {
            total += sizeof(StateBucket);
            
            if (sb->value) {
                total += sizeof(CharMap);
                total += sb->value->capacity * sizeof(CharBucket*);

                for (j = 0; j < sb->value->capacity; j++) {
                    cb = sb->value->buckets[j];
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

static inline int isValidCharacter(char c) {
    // Letters and digits
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
        return 1;
    }

    // Special characters and whitespace
    switch (c) {
    case '+':
    case '-':
    case '_':
    case ':':
    case '(':
    case ')':
    case '"':
    case ' ':
    case '.':
        return 1;
    default:
        return 0;
    }
}

void addKeyword(TransitionTable* table, const char* word, TokenType token) {
    unsigned short current, nextId;
    int i;

    current = 0;
    for (i = 0; word[i] != '\0'; i++) {
        nextId = getState(table, current, word[i]);
        if (nextId == STATE_ERROR) {
            nextId = ++table->stateCounter;
            insertTransition(table, current, word[i], nextId);
        }
        current = nextId;
    }
    setToken(table, current, token);
}

void buildTransitionTable(TransitionTable* table) {
    int i;

    // WHITESPACES
    insertTransition(table, 0, ' ', 0);
    insertTransition(table, 0, '\n', 0);
    insertTransition(table, 0, '\t', 0);
    insertTransition(table, 0, '\r', 0);
    insertTransition(table, 0, '\v', 0);
    insertTransition(table, 0, '\f', 0);
    setToken(table, 0, TOKEN_IDLE);

    // COMMENTS
    insertTransition(table, 0, 'N', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, 'O', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, 'T', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, 'E', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, ':', STATE_COMMENT);
    insertTransition(table, STATE_COMMENT, '\n', 0);

    // KEYWORDS
    for (i = 0; i < sizeof(keywordTable) / sizeof(keywordTable[0]); i++)
        addKeyword(table, keywordTable[i].keyword, keywordTable[i].type);

    // NUMBERS
    ++table->stateCounter;
    for (i = '0'; i <= '9'; i++) {
        insertTransition(table, 0, i, table->stateCounter);
        insertTransition(table, table->stateCounter, i, table->stateCounter);
    }
    insertTransition(table, 0, '-', table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_NUMBER);

    // SPECIAL CHARACTERS
    insertTransition(table, 0, '(', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_LPAREN);
    insertTransition(table, 0, ')', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_RPAREN);
    insertTransition(table, 0, '"', STATE_TEXT);
    insertTransition(table, STATE_TEXT, '"', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_STRING);
    insertTransition(table, 0, ':', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_COLON);
    insertTransition(table, 0, '+', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_PLUS);
    insertTransition(table, 0, '.', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_EOS);
    insertTransition(table, 0, EOF, ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_EOF);
    setToken(table, ++table->stateCounter, TOKEN_IDENT);
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
    unsigned short nextState;
    TokenType type;

    nextState = getState(lexerFSM->transitionTable, lexerFSM->currentState, input);

    if (nextState != STATE_ERROR) {
        lexerFSM->currentState = nextState;
        return TOKEN_IDLE;
    }
    
    if (lexerFSM->currentState == STATE_COMMENT || lexerFSM->currentState == STATE_TEXT)
        return TOKEN_IDLE;


    if (!isDelimiter(input)) {
        if (!isValidCharacter(input)) {
            printf("Error: INVALID CHARACTER");
            exit(EXIT_FAILURE);
        }

        lexerFSM->currentState = lexerFSM->transitionTable->stateCounter;
        return TOKEN_IDLE;
    }

    type = getTokenType(lexerFSM->transitionTable, lexerFSM->currentState);
    lexerFSM->currentState = 0;

    return type;
}

void freeLexerFSM(LexerFSM* lexerFSM) {
    freeTransitionTable(lexerFSM->transitionTable);
    free(lexerFSM);
}