#include "ScopeStack.h"
#include <stdlib.h>
#include <stdio.h>

ScopeStack* initScopeStack() {
	return NULL;
}

void push(ScopeStack** stack, ScopeStackItem item) {
	ScopeStack* node = (ScopeStack*)malloc(sizeof(ScopeStack));
	if (!node) {
		printf(RED "Error: Failed to allocate memory for Symbol bucket\n" RESET);
		exit(EXIT_FAILURE);
	}

	node->value = item;
	node->next = *stack;

	*stack = node;
}

ScopeStackItem pop(ScopeStack** stack) {
	if (stack == NULL || *stack == NULL) {
		printf(RED "Error: Can't pop from empty stack\n" RESET);
		exit(EXIT_FAILURE);
	}

	ScopeStack* head = *stack;
	ScopeStackItem value = head->value;

	*stack = head->next;

	free(head);
	return value;
}

ScopeStackItem peek(ScopeStack* stack) {
	return stack->value;
}