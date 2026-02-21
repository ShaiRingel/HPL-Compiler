#include "Compiler.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
	Compiler* compiler = initCompiler(argv[1]);

	startCompiler(compiler);
	LexerFSM* fsm = initLexerFSM();
	size_t bytes = calculateTransitionTableMemory(fsm->transitionTable);

	printf("Transition table memory usage: %zu bytes (%.2f KB)\n",
		bytes, bytes / 1024.0);

	return 0;
}
