#pragma once
#include "Global.h"
#include "ActionTable.h"
#include "GotoTable.h"

typedef struct {
	ActionTable* actionTable;
	GotoTable* gotoTable;
} ParsingTable;

// Initialize parsingTables components
ParsingTable* initParsingTable();

// Frees the entire ParsingTable and all associated memory
void freeParsingTable(ParsingTable* table);