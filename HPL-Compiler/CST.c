#include "CST.h"
#include <stdlib.h>
#include <stdio.h>

CSTNode* createASTNode(SymbolType type, int symbol, Token* token) {
    CSTNode* node = (CSTNode*)malloc(sizeof(CSTNode));
    if (!node) {
        printf(RED "ERROR: Failed to allocate memory for CST Node\n" RESET);
        exit(EXIT_FAILURE);
    }

    node->type = type;
    node->symbol = symbol;
    node->token = token;

    node->firstChild = NULL;
    node->nextSibling = NULL;

    return node;
}

void addChild(CSTNode* parent, CSTNode* child) {
    if (!parent || !child) return;

    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    }
    else {
        CSTNode* sibling = parent->firstChild;

        while (sibling->nextSibling)
            sibling = sibling->nextSibling;

        sibling->nextSibling = child;
    }
}

void freeAST(CSTNode* node) {
    if (!node) return;

    freeAST(node->firstChild);
    freeAST(node->nextSibling);

    if (node->token != NULL) {
        free(node->token->lexeme);
        free(node->token);
    }

    free(node);
}

void printASTLevel(CSTNode* node, int level) {
    if (!node) return;

    CSTNode* child;
    int i;

    for (i = 0; i < level; i++) printf(" ");

    if (node->type == TERMINAL && node->token)
        printf("Leaf: %s\n", node->token->lexeme);
    else
        printf("Node: %s\n", nonTerminalNames[node->symbol - NON_TERMINAL_PROG]);

    child = node->firstChild;
    while (child != NULL) {
        printASTLevel(child, level + 1);
        child = child->nextSibling;
    }
}

void printAST(CSTNode* node) {
    if (!node) {
        printf("CST is empty.\n");
        return;
    }
    printf("\n--- Abstract Syntax Tree ---\n");
    printASTLevel(node, 0);
    printf("----------------------------\n\n");
}