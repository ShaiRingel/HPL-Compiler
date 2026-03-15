#pragma once
#include "Compiler.h"

typedef enum {
    ERROR_LEXICAL,
    ERROR_SYNTAX,
    ERROR_SEMANTIC,
    ERROR_INTERNAL
} ErrorType;

// 
void reportError(ErrorType type, const char* format, ...);

// 
int attemptSingleTokenInsertion(Compiler* compiler, Token* currentToken, int* next);