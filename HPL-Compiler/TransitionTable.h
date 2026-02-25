#pragma once
#include "Global.h"

#define STATE_CAPACITY 251
#define INITIAL_CHAR_CAPACITY 1
#define EXPANDED_CHAR_CAPACITY 11
#define STATE_ERROR (unsigned short) -1
#define STATE_COMMENT (unsigned short) -2
#define STATE_TEXT (unsigned short) -3

typedef struct CharBucket {
    char key;
    unsigned short value; // Next state
    struct CharBucket* next;
} CharBucket;

typedef struct {
    CharBucket** buckets;
    int capacity;
} CharMap;

typedef struct StateBucket {
    int key;              // State ID
    CharMap* value;       // Transitions
    TokenType token;      // Associated token (if any)
    struct StateBucket* next;
} StateBucket;

typedef struct {
    StateBucket** buckets;
    int stateCounter;
    int capacity;
} TransitionTable;

TransitionTable* initTransitionTable();
void insertTransition(TransitionTable* table, unsigned short state, char symbol, unsigned short newState);
unsigned short getState(const TransitionTable* table, unsigned short state, char symbol);
void setToken(TransitionTable* table, unsigned short state, TokenType token);
TokenType getTokenType(const TransitionTable* table, unsigned short state);
void freeTransitionTable(TransitionTable* table);