#pragma once
#include "Global.h"
#include "ParsingDefinitions.h"

typedef struct CSTNode {
    SymbolType type;
    Token* token;
    int symbol;

    struct CSTNode* firstChild;
    struct CSTNode* nextSibling;
} CSTNode;

CSTNode* createASTNode(SymbolType type, int symbol, Token* token);
void addChild(CSTNode* parent, CSTNode* child);
void freeAST(CSTNode* node);
void printAST(CSTNode* node);