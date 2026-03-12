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

    current = STATE_START;
    for (i = 0; word[i] != '\0'; i++) {
        nextId = getState(table, current, word[i]);
        if (nextId == STATE_ERROR) {
            nextId = ++table->stateCounter;
            insertTransition(table, current, word[i], nextId);
            setToken(table, nextId, TOKEN_IDENT);
        }
        current = nextId;
    }
    setToken(table, current, token);
}

void buildTransitionTable(TransitionTable* table) {
    char c;
    int i;

    // WHITESPACES
    insertTransition(table, STATE_START, ' ', STATE_START);
    insertTransition(table, STATE_START, '\n', STATE_START);
    insertTransition(table, STATE_START, '\r', STATE_START);
    insertTransition(table, STATE_START, '\v', STATE_START);
    insertTransition(table, STATE_START, '\f', STATE_START);
    setToken(table, STATE_START, TOKEN_IDLE);

    // COMMENTS
    insertTransition(table, STATE_START, 'N', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, 'O', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, 'T', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, 'E', table->stateCounter + 1);
    insertTransition(table, ++table->stateCounter, ':', STATE_COMMENT);
    insertTransition(table, STATE_COMMENT, '\n', STATE_START);

    // KEYWORDS
    for (i = 0; i < sizeof(keywordTable) / sizeof(keywordTable[0]); i++)
        addKeyword(table, keywordTable[i].keyword, keywordTable[i].type);

    // NUMBERS
    ++table->stateCounter;
    for (c = '0'; c <= '9'; c++) {
        insertTransition(table, STATE_START, c, table->stateCounter);
        insertTransition(table, table->stateCounter, c, table->stateCounter);
    }
    insertTransition(table, STATE_START, '-', table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_NUMBER);

    // SPECIAL CHARACTERS
    insertTransition(table, STATE_START, '\t', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_INDENT);
    insertTransition(table, STATE_START, '(', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_LPAREN);
    insertTransition(table, STATE_START, ')', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_RPAREN);
    insertTransition(table, STATE_START, '"', STATE_TEXT);
    insertTransition(table, STATE_TEXT, '"', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_STRING);
    insertTransition(table, STATE_START, ':', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_COLON);
    insertTransition(table, STATE_START, '+', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_PLUS);
    insertTransition(table, STATE_START, '.', ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_EOS);
    insertTransition(table, STATE_START, EOF, ++table->stateCounter);
    setToken(table, table->stateCounter, TOKEN_EOF);
    
    for (i = 'A'; i < 'Z'; i++) {
        if (getState(table, STATE_START, i) == STATE_ERROR)
            insertTransition(table, STATE_START, i, STATE_IDENT);
        if (getState(table, STATE_START, i | 0x20) == STATE_ERROR)
            insertTransition(table, STATE_START, i | 0x20, STATE_IDENT);
    }

    insertTransition(table, STATE_START, '_', STATE_IDENT);
    setToken(table, STATE_IDENT, TOKEN_IDENT);
}

LexerFSM* initLexerFSM() {
    LexerFSM* fsm;

    fsm = (LexerFSM*)malloc(sizeof(LexerFSM));
    if (!fsm) exit(EXIT_FAILURE);

    fsm->currentState = STATE_START;
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

        lexerFSM->currentState = STATE_IDENT;
        return TOKEN_IDLE;
    }

    type = getTokenType(lexerFSM->transitionTable, lexerFSM->currentState);
    lexerFSM->currentState = STATE_ACCEPT;

    return type;
}

void freeLexerFSM(LexerFSM* lexerFSM) {
    freeTransitionTable(lexerFSM->transitionTable);
    free(lexerFSM);
}