#include "ParsingStack.h"
#include <stdlib.h>
#include <stdio.h>

static void pop(ParsingStack** stack) {
    if (stack == NULL || *stack == NULL) {
        printf(RED "Error: Can't pop from empty stack\n" RESET);
        exit(EXIT_FAILURE);
    }

    ParsingStack* node = *stack;

    *stack = node->next;
    free(node);
}

ParsingStack* initParsingStack() {
    ParsingStack* stack = (ParsingStack*)malloc(sizeof(ParsingStack));
    if (!stack) {
        printf(RED "Error: Failed to allocate memory for Parsing Stack\n" RESET);
        exit(EXIT_FAILURE);
    }

    ParsingStackItem item = { 0 };
    shift(&stack, item);

    return stack;
}

void shift(ParsingStack** stack, ParsingStackItem item) {
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

ParsingStackItem lookahed(ParsingStack* stack) {
    if (stack == NULL) {
        printf(RED "Error: Can't peek from empty stack\n" RESET);
        exit(EXIT_FAILURE);
    }
    return stack->value;
}