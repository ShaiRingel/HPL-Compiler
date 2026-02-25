#pragma once

#include "Global.h"
#include "ParsingStack.h"

#define STATES_CAPACITY 23
#define GRAMMAR_CAPACITY 53

typedef enum {
    SHIFT,
    REDUCE
} ActionType;

typedef union {
    ParsingStackItem shift_item;
    int reduce_count;
} ActionData;

typedef struct ParserAction {
    unsigned short state;
    ActionType type;
    ActionData data;
    struct ParserAction* next;
} ParserAction;

typedef struct {
    ParserAction** buckets;
    int capacity;
} ActionHashMap;

typedef struct ActionTableEntry {
    Token token;
    ActionHashMap* actions;
    struct ActionTableEntry* next;
} ActionTableEntry;

typedef struct {
    ActionTableEntry** buckets;
    int capacity;
} ActionTable;

// Initializes and allocates a new ActionTable
ActionTable* initActionTable();

// Inserts or updates a SHIFT/REDUCE action for a given token and state
void createAction(ActionTable* table, Token token, unsigned short state,
    ActionType type, ActionData data);

// Retrieves the parser action for a given token and state (NULL if not found)
ParserAction* getAction(ActionTable* table, Token token, unsigned short state);

// Frees the entire ActionTable and all associated memory
void freeActionTable(ActionTable* table);