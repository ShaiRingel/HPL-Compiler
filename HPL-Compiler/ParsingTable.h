#pragma once
#include "Global.h"

#define CAPACITY 100

typedef struct {
	Token key;
	void* function;
	struct ActionBucket* next;
} ActionBucket;

typedef struct {
	Token key;
	void* function;
	struct ActionBucket* next;
} ActionBucket2;

typedef struct ActionTable {
	ActionBucket** buckets;
	int capacity;
} ActionTable;

typedef struct {
	Token key;
	unsigned short state;
	struct ActionTable* next;
} GotoBucket;

typedef struct {
	Token key;
	unsigned short state;
	struct GotoBucket* next;
} GotoBucket2;

typedef struct GotoTable {
	GotoBucket** buckets;
	int capacity;
} GotoTable;

typedef struct {
	ActionTable* actionTable;
	GotoTable* gotoTable;
} ParsingTable;

// Initialize parsingTables components
ParsingTable* initParsingTable();
// Creates a new action in the Action Table
void createAction(ParsingTable* table, Token token, unsigned short state, void* func);
// Creates a new goto in the Goto Table
void createGoto(ParsingTable* table, Token token, unsigned short state, unsigned short newState);
// Creates a new action in the Action Table
void* getAction(ParsingTable* table, Token token, unsigned short state);
// Creates a new goto in the Goto Table
unsigned short getGoto(ParsingTable* table, Token token, unsigned short state);