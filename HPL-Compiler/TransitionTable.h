#pragma once
#include "Global.h"

#define STATE_CAPACITY 251
#define INITIAL_CHAR_CAPACITY 1
#define EXPANDED_CHAR_CAPACITY 11

typedef enum {
    STATE_ACCEPT = (unsigned short) -5,
    STATE_ERROR,
    STATE_IDENT,
    STATE_COMMENT,
    STATE_TEXT
} SpecialStates;

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

// Initializes and allocates a new Transition
TransitionTable* initTransitionTable();

// Adds a transition from `state` on input `symbol` to `newState`
void insertTransition(TransitionTable* table, unsigned short state, char symbol, unsigned short newState);

// Retrieves the next state for a given (state, symbol) pair
unsigned short getState(const TransitionTable* table, unsigned short state, char symbol);

// Associates a token type with a specific state
void setToken(TransitionTable* table, unsigned short state, TokenType token);

// Returns the token type associated with a state
TokenType getTokenType(const TransitionTable* table, unsigned short state);

// Frees the entire TransitionTable and all associated memory
void freeTransitionTable(TransitionTable* table);