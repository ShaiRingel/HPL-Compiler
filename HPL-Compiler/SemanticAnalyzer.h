#pragma once
#include "SymbolTable.h"
#include "ScopeStack.h"

typedef struct {
	SymbolTable* global;
	ScopeStack* scopes;
} SemanticAnalyzer;

// 
SemanticAnalyzer* initSemanticAnalyzer();
// 
