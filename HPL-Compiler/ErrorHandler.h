#pragma once

typedef enum {
    ERROR_LEXICAL,
    ERROR_SYNTAX,
    ERROR_SEMANTIC,
    ERROR_INTERNAL
} ErrorType;

void reportError(ErrorType type, const char* format, ...);