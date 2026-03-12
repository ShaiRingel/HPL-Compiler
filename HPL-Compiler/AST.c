#include "AST.h"
#include <stdlib.h>
#include <stdio.h>

ASTNode* createASTNode(SymbolType type, int symbol, Token* token) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        printf(RED "ERROR: Failed to allocate memory for AST Node\n" RESET);
        exit(EXIT_FAILURE);
    }

    node->type = type;
    node->symbol = symbol;
    node->token = token;

    node->firstChild = NULL;
    node->nextSibling = NULL;

    return node;
}

void addChild(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;

    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    }
    else {
        ASTNode* sibling = parent->firstChild;

        while (sibling->nextSibling != NULL)
            sibling = sibling->nextSibling;

        sibling->nextSibling = child;
    }
}

void freeAST(ASTNode* node) {
    if (!node) return;

    freeAST(node->firstChild);
    freeAST(node->nextSibling);
    free(node);
}

void printASTLevel(ASTNode* node, int level) {
    if (!node) return;

    ASTNode* child;
    int i;

    for (i = 0; i < level; i++) printf("  ");

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

void printAST(ASTNode* node) {
    if (!node) {
        printf("AST is empty.\n");
        return;
    }
    printf("\n--- Abstract Syntax Tree ---\n");
    printASTLevel(node, 0);
    printf("----------------------------\n\n");
}