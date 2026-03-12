#pragma once
#include "Global.h"
#include "HashingFunctions.h"
#include "ParsingDefinitions.h"

#define CAPACITY_STATES 179
#define CAPACITY_SYMBOLS 11

typedef enum {
    ACTION_ERROR = -1,
    ACTION_SHIFT,
    ACTION_REDUCE,
    ACTION_GOTO,
    ACTION_ACCEPT
} ActionType;

typedef struct {
    ActionType type;
    int value;
} ParseAction;

typedef struct SymbolNode {
    int symbol;
    ParseAction action;
    struct SymbolNode* next;
} ActionNode;

typedef struct {
    ActionNode** symbolBuckets;
    int capacity;
} StateRow;

typedef struct StateNode {
    unsigned short stateId;
    StateRow* row;
    struct StateNode* next;
} StateNode;

typedef struct {
    StateNode** stateBuckets;
    int capacity;
} ParsingTable;

ParsingTable* initParsingTable(void);
void insertTableEntry(ParsingTable* table, unsigned short stateId, int symbol, ActionType type, int value);
ParseAction getEntry(ParsingTable* table, unsigned short stateId, int symbol);
void freeParsingTable(ParsingTable* table);

int getRuleLength(int ruleId);
int getLHS(int ruleId);