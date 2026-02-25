#pragma once
#include "Global.h"

#define CAPACITY 100

typedef struct {
	int value;
	TokenType type;
} SymbolData;

typedef struct SymbolEntry {
	char* key;
	SymbolData data;
	struct SymbolEntry* next;
} SymbolEntry;

typedef struct {
	SymbolEntry** buckets;
	int capacity;
} SymbolTable;

// Initializes and allocates a new SymbolTable
SymbolTable* initSymbolTable();

// Inserts a new symbol into the table
void putSymbol(SymbolTable* table, char* key, SymbolData data);

// Removes a symbol from the table by key
void removeSymbol(SymbolTable* table, const char* key);

// Retrieves the data associated with a given key
SymbolData getSymbol(SymbolTable* table, char* key);

// Frees all memory associated with the SymbolTable
void freeSymbolTable(SymbolTable* table);