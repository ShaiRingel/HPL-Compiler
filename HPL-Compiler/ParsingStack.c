#include "ParsingStack.h"
#include "ErrorHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

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
        reportError(ERROR_INTERNAL, "Invalid stack pointer");
    }
    ParsingStack* node = malloc(sizeof(ParsingStack));

    if (!node)
        reportError(ERROR_INTERNAL, "Failed to allocate memory for Parsing Stack");

    assert(node);
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
        reportError(ERROR_INTERNAL, "Failed to allocate memory for children array in reduce");

    assert(children);

    for (i = amount - 1; i >= 0; i--) {
        if (!stack || !*stack) {
            reportError(ERROR_SYNTAX, "Reduce operation attempted beyond stack size");
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
    if (!stack)
        reportError(ERROR_INTERNAL, "Cannot peek from empty stack");

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