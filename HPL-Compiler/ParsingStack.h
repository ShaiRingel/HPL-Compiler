#pragma once
#include "Global.h"

typedef struct {
	Token token;
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
void reduce(ParsingStack** stack, int ammount);
// Returns the item currently on the top of the stack
ParsingStackItem lookahed(ParsingStack* stack);