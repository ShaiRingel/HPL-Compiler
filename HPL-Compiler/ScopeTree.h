#pragma once
#include "SymbolTable.h"

typedef struct ScopeNode {
    SymbolTable* symbols;          // Local symbols for this specific scope
    struct ScopeNode* parent;      // Pointer to the enclosing scope
    struct ScopeNode** children;   // Array of nested scopes
    int numChildren;
    int capacity;
} ScopeNode;

// Initialize a new scope node
ScopeNode* createScope(ScopeNode* parent);

// Add a nested child scope to a parent scope
void addChildScope(ScopeNode* parent, ScopeNode* child);

// Lookup a symbol starting from the current scope and moving up to the global scope
SymbolData lookupSymbol(ScopeNode* currentScope, char* key);

// Free the entire tree recursively
void freeScopeTree(ScopeNode* node);