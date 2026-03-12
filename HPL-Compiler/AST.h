#pragma once
#include "Global.h"
#include "ParsingDefinitions.h"

typedef struct ASTNode {
    SymbolType type;
    int symbol;
    Token* token;

    struct ASTNode* firstChild;
    struct ASTNode* nextSibling;
} ASTNode;

ASTNode* createASTNode(SymbolType type, int symbol, Token* token);
void addChild(ASTNode* parent, ASTNode* child);
void freeAST(ASTNode* node);
void printAST(ASTNode* node);