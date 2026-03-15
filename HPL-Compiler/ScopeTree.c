#include "ScopeTree.h"
#include "Global.h"
#include <stdlib.h>
#include <stdio.h>

ScopeNode* createScope(ScopeNode* parent) {
    ScopeNode* node = (ScopeNode*)malloc(sizeof(ScopeNode));
    if (!node) {
        printf(RED "Error: Failed to allocate ScopeNode\n" RESET);
        exit(EXIT_FAILURE);
    }

    // Initialize the symbol table for this scope
    node->symbols = initSymbolTable();
    node->parent = parent;

    node->capacity = 4; // Start with a small capacity for nested scopes
    node->numChildren = 0;
    node->children = (ScopeNode**)malloc(node->capacity * sizeof(ScopeNode*));

    if (!node->children) {
        printf(RED "Error: Failed to allocate children for ScopeNode\n" RESET);
        exit(EXIT_FAILURE);
    }

    return node;
}

void addChildScope(ScopeNode* parent, ScopeNode* child) {
    if (!parent || !child) return;

    // Resize array if we have too many nested scopes
    if (parent->numChildren >= parent->capacity) {
        parent->capacity *= 2;
        parent->children = (ScopeNode**)realloc(parent->children, parent->capacity * sizeof(ScopeNode*));

        if (!parent->children) {
            printf(RED "Error: Failed to reallocate children array\n" RESET);
            exit(EXIT_FAILURE);
        }
    }

    parent->children[parent->numChildren++] = child;
}

SymbolData lookupSymbol(ScopeNode* currentScope, char* key) {
    if (!currentScope) {
        SymbolData empty = { 0 };
        return empty;
    }

    SymbolData data = getSymbol(currentScope->symbols, key);

    if (data.type == TYPE_NONE && currentScope->parent)
        return lookupSymbol(currentScope->parent, key);

    return data;
}

void freeScopeTree(ScopeNode* node) {
    if (!node) return;
    int i;
    
    for (i = 0; i < node->numChildren; i++)
        freeScopeTree(node->children[i]);

    freeSymbolTable(node->symbols);
    free(node->children);
    free(node);
}