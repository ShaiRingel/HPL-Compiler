#include "SemanticAnalyzer.h"
#include <stdlib.h>
#include <stdio.h>

SemanticAnalyzer* initSemanticAnalyzer() {
	SemanticAnalyzer* analyzer = (SemanticAnalyzer*)malloc(sizeof(SemanticAnalyzer));
	if (!analyzer) {
		printf(RED "Error: Failed to allocate Semantic Analyzer" RESET);
		exit(EXIT_FAILURE);
	}

	analyzer->global = initSymbolTable();
	analyzer->scopes = initScopeStack();

	return analyzer;
}


void freeSemanticAnalyzer(SemanticAnalyzer* analyzer) {
	freeSymbolTable(analyzer->global);
	freeScopeStack(analyzer->scopes);
	free(analyzer);
}