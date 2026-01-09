#pragma once

#include "Lexer.h"

typedef enum {
    AST_PROGRAM,
    AST_SHOW,
    AST_LET,
    AST_SET,
    AST_INCREASE,
    AST_DECREASE,
    AST_MULTIPLY,
    AST_DIVIDE,
    AST_NUMBER,
    AST_IDENTIFIER,
    AST_BINARY
} ASTNodeType;

typedef enum {
    DT_INTEGER,
    DT_REAL,
    DT_IDENTIFIER
} DataType;



typedef struct astNode{
    ASTNodeType type;
    DataType dataType;

    union {
        TokenType operat;
        int integer;
        double real;
        char* identifier;
    } value;

	struct astNode *left;
	struct astNode *right;
    struct astNode* (*next);
} ASTNode;

// 
ASTNode* createNode(ASTNodeType type);
// 
void freeAST(ASTNode *node);
// 
void printAST(ASTNode *node);