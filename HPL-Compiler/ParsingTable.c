#include "ParsingTable.h"
#include <stdlib.h>
#include <stdio.h>

ParsingTable* initParsingTable() {
	ParsingTable* table = (ParsingTable*)malloc(sizeof(ParsingTable));
	if (!table) {
		printf(RED "Error: Failed to allocate memory for Parsing Table\n" RESET);
		exit(EXIT_FAILURE);
	}

	table->actionTable = initActionTable();
	table->gotoTable = initGotoTable();

	return table;
}