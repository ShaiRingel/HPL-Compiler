#include "ParsingStack.h"
#include <stdlib.h>
#include <stdio.h>

void pop(ParsingStack** stack) {
    if (*stack == NULL) {
        printf(RED "Error: Can't pop from empty stack\n" RESET);
        exit(EXIT_FAILURE);
    }

    ParsingStack* node = *stack;

    *stack = node->next;
    free(node);
}

ParsingStack* initStack() {
    return NULL;
}

void shift(ParsingStack** stack, StackItem item) {
    ParsingStack* node = (ParsingStack*)malloc(sizeof(ParsingStack));
    if (!node) {
        printf(RED "Error: Failed to allocate memory for lexer\n" RESET);
        exit(EXIT_FAILURE);
    }

    node->value = item;
    node->next = *stack;
    *stack = node;
}

void reduce(ParsingStack** stack, int ammount) {
    int i;

    for (i = 0; i < ammount; i++)
        pop(stack);
}

StackItem peek(ParsingStack* stack) {
    if (stack == NULL) {
        printf(RED "Error: Can't peek from empty stack\n" RESET);
        exit(EXIT_FAILURE);
    }
    return stack->value;
}