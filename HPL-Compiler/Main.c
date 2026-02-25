#include "Compiler.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Too many arguments passed!");
        exit(EXIT_FAILURE);
    }

    Compiler* compiler = initCompiler(argv[1]);
    startCompiler(compiler);

    return 0;
}