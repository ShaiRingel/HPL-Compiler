#include "Compiler.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Ammount of arguments passed is incorrect, need 1!");
        exit(EXIT_FAILURE);
    }

    // Compiler* compiler = initCompiler("LexerTest.hpl");
    Compiler* compiler = initCompiler(argv[1]);
    startCompiler(compiler);

    freeCompiler(compiler);

    return 0;
}