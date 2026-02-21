#pragma once
#include "Global.h"

typedef struct {
	Token token;
	unsigned short state;
} StackItem;

typedef struct ParsingStack{
	StackItem value;
	struct ParsingStack *next;
} ParsingStack;

// Initialize stacks components
ParsingStack* initStack();
// Pushes the item given as parameter to the top of the stack
void shift(ParsingStack** stack, StackItem item);
// Removes the top items in the stack according to the ammount
void reduce(ParsingStack** stack, int ammount);
// Returns the item currently on the top of the stack
StackItem peek(ParsingStack* stack);