#include "ParsingStack.h"
#include <stdlib.h>
#include <stdio.h>

static void pop(ParsingStack** stack) {
    if (!stack || !*stack) {
        printf(RED "Error: Can't pop from empty stack\n" RESET);
        exit(EXIT_FAILURE);
    }

    ParsingStack* node = *stack;

    *stack = node->next;
    free(node);
}

ParsingStack* initParsingStack() {
    return NULL;
}

void shift(ParsingStack** stack, ParsingStackItem item) {
    if (!stack) {
        printf(RED "Error: Invalid stack pointer\n" RESET);
        exit(EXIT_FAILURE);
    }
    ParsingStack* node = malloc(sizeof(ParsingStack));

    if (!node) {
        printf(RED "Error: Failed to allocate memory for Parsing Stack\n" RESET);
        exit(EXIT_FAILURE);
    }

    node->value = item;
    node->next = *stack;
    *stack = node;
}


void reduce(ParsingStack** stack, int amount) {
    int i;

    for (i = 0; i < amount; i++) {
        if (!stack || !*stack) {
            printf(RED "Error: reduce beyond stack size\n" RESET);
            exit(EXIT_FAILURE);
        }
        pop(stack);
    }
}

ParsingStackItem lookahed(ParsingStack* stack) {
    if (!stack) {
        printf(RED "Error: Can't peek from empty stack\n" RESET);
        exit(EXIT_FAILURE);
    }
    return stack->value;
}

void freeParsingStack(ParsingStack* stack) {
    while (stack) {
        ParsingStack* next = stack->next;
        free(stack);
        stack = next;
    }
}
