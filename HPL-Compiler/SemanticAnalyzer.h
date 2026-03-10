#pragma once
#include "SymbolTable.h"
#include "ScopeStack.h"

typedef struct {
	SymbolTable* global;
	ScopeStack* scopes;
} SemanticAnalyzer;

// Initializes and allocates a new SemanticAnalyzer
SemanticAnalyzer* initSemanticAnalyzer();

// Frees the entire SemanticAnalyzer and all associated memory
void freeSemanticAnalyzer(SemanticAnalyzer* analyzer);