#include "Compiler.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    Compiler* compiler = initCompiler(argv[1]);
    startCompiler(compiler);


    return 0;
}