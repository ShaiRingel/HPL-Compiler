#include "ErrorHandler.h"
#include "Global.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

void printErrorPrefix(ErrorType type) {
    switch (type) {
    case ERROR_LEXICAL:  printf("[Lexical Error] "); break;
    case ERROR_SYNTAX:   printf("[Syntax Error] "); break;
    case ERROR_SEMANTIC: printf("[Semantic Error] "); break;
    case ERROR_INTERNAL: printf("[Internal Error] "); break;
    default:           printf("[General Error] "); break;
    }
}

void reportError(ErrorType type, const char* format, ...) {
    va_list args;

    va_start(args, format);
    printf(RED);
    printErrorPrefix(type);
    vprintf(format, args);
    printf(RESET "\n");
    va_end(args);

    exit(EXIT_FAILURE);
}