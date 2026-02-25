#pragma once
#include "SymbolTable.h"

typedef SymbolTable ScopeStackItem;

typedef struct ScopeStack {
	ScopeStackItem value;
	struct ScopeStack* next;
} ScopeStack;

// Initialize stacks components
ScopeStack* initScopeStack();
// Pushes a new Symbol Table (new scope) to the stack
void push(ScopeStack** stack, ScopeStackItem item);
// Removes the item on top of the stack and return it
ScopeStackItem pop(ScopeStack** stack);
// Returns the item currently on the top of the stack
ScopeStackItem peek(ScopeStack* stack);