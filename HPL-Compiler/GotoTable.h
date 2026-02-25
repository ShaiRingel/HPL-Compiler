#pragma once

#include "Global.h"

#define STATE_CAPACITY 23
#define GRAMMAR_CAPACITY 53

typedef struct ParserGoto {
	unsigned short state;
	unsigned short newState;
	struct ParserGoto* next;
} ParserGoto;

typedef struct {
	ParserGoto** buckets;
	int capacity;
} GotoHashMap;

typedef struct GotoTableEntry {
	Token token;
	GotoHashMap* gotos;
	struct GotoTableEntry* next;
} GotoTableEntry;

typedef struct {
	GotoTableEntry** buckets;
	int capacity;
} GotoTable;

// Initializes the GotoTables components
GotoTable* initGotoTable();
// Creates a new goto in the GotoTable
void createGoto(GotoTable* table, Token token, unsigned short state, unsigned short newState);
// 
static void freeGotoHashMap(GotoHashMap* map);
// 
void freeGotoTable(GotoTable* table);