#include "Compiler.h"
#include <stdlib.h>

Compiler* initCompiler() {
	Compiler* compiler = (Compiler*) malloc(sizeof(Compiler));
	if (!compiler) {
		printf(RED "Error: Failed to allocate memory for compiler\n" RESET);
		exit(EXIT_FAILURE);
	}

	return compiler;
}