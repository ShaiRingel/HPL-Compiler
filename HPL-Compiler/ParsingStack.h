#pragma once
#include "Global.h"
#include "AST.h"

typedef struct {
    Token token;
    ASTNode* astNode;
    unsigned short state;
} ParsingStackItem;

typedef struct ParsingStack{
	ParsingStackItem value;
	struct ParsingStack *next;
} ParsingStack;

// Initialize stacks components
ParsingStack* initParsingStack();

// Pushes the item given as parameter to the top of the stack
void shift(ParsingStack** stack, ParsingStackItem item);

// Removes the top items in the stack according to the ammount
ASTNode* reduce(ParsingStack** stack, int ammount, int lhs);

// Returns the item currently on the top of the stack
ParsingStackItem lookahed(ParsingStack* stack);

// Frees the entire ParsingStack and all associated memory
void freeParsingStack(ParsingStack* stack);

void printStack(ParsingStack* stack);