#pragma once

typedef enum {
    NODE_PROGRAM,
    NODE_VAR_DECL,
    NODE_TYPE_INT,
    NODE_TYPE_REAL,
    NODE_CONST_DECL,
    NODE_ASSIGN_STMT,
    NODE_SHOW,
    NODE_IF_STMT,
    NODE_WHILE_STMT,
    NODE_BLOCK,
    NODE_BIN_OP,
    NODE_BOOL_OP,
    NODE_IDENTIFIER,
    NODE_LITERAL_INT,
    NODE_LITERAL_REAL
} NodeType;

typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV,
    OP_EQ, OP_NEQ, OP_LT, OP_GT, OP_LE, OP_GE,
    OP_AND, OP_OR
} OpType;

typedef struct ASTNode {
    NodeType type;

    // For Operators
    OpType op;

    // For Literals (Values)
    union {
        int intValue;
        double realValue;
        char* stringValue; // For Identifiers
    } val;

    // Pointers to children
    struct ASTNode* left;
    struct ASTNode* right;
    struct ASTNode* condition;
    struct ASTNode* body;
    struct ASTNode* elseBody;
    struct ASTNode* next;

} ASTNode;

// 
ASTNode* createNode(NodeType type);
// 
ASTNode* createBinOpNode(OpType op, ASTNode* left, ASTNode* right);
// 
ASTNode* createLiteralNode(int val);
// 
ASTNode* createIdNode(char* name);
// 
ASTNode* createAssignmentNode(char* varName, ASTNode* expression);
// 
ASTNode* createVariableDeclarationNode(char* varName);
// 
ASTNode* createIfNode(ASTNode* condition, ASTNode* body, ASTNode* elseBody);
// 
void freeAST(ASTNode* root);
// 
void printAST(ASTNode* node, int level);