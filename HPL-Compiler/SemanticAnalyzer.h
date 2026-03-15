#pragma once
#include "ScopeTree.h"
#include "CST.h"

typedef struct {
    ScopeNode* rootScope;
    ScopeNode* currentScope;
} SemanticAnalyzer;

// Initializes and allocates a new SemanticAnalyzer
SemanticAnalyzer* initSemanticAnalyzer();

// Frees the entire SemanticAnalyzer and all associated memory
void freeSemanticAnalyzer(SemanticAnalyzer* analyzer);

void analyzeAST(SemanticAnalyzer* analyzer, CSTNode* root);

void analyzeNode(SemanticAnalyzer* analyzer, CSTNode* node);