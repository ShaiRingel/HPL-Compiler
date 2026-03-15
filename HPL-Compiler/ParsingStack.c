#include "ParsingStack.h"
#include <stdlib.h>
#include <stdio.h>

CSTNode* pop(ParsingStack** stack) {
    if (!stack || !*stack) return NULL;
    ParsingStack* node = *stack;
    CSTNode* astNode = node->value.astNode;
    *stack = node->next;
    free(node);
    return astNode;
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


CSTNode* reduce(ParsingStack** stack, int amount, int lhs) {
    if (amount == 0)
        return createASTNode(NON_TERMINAL, lhs, NULL);

    int i;
    CSTNode* parent;

    CSTNode** children = malloc(sizeof(CSTNode*) * amount);
    if (!children)
        exit(EXIT_FAILURE);


    for (i = amount - 1; i >= 0; i--) {
        if (!stack || !*stack) {
            printf(RED "Error: reduce beyond stack size\n" RESET);
            exit(EXIT_FAILURE);
        }
        
        children[i] = pop(stack);
    }

    parent = createASTNode(NON_TERMINAL, lhs, NULL);

    for (i = 0; i < amount; i++)
        addChild(parent, children[i]);

    free(children);
    return parent;
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

static void printStackRecursive(ParsingStack* node) {
    if (node == NULL) return;

    printStackRecursive(node->next);

    if (node->value.state == 0 && node->next == NULL)
        printf("%d ", node->value.state);
    else
        printf("%s %d ", node->value.token.lexeme, node->value.state);
}

void printStack(ParsingStack* stack) {
    printStackRecursive(stack); printf("\n\n");
}