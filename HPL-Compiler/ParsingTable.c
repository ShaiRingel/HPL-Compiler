#include "ParsingTable.h"
#include <stdlib.h>
#include <stdio.h>

/*unsigned hashState(unsigned short state) {
	return (unsigned)state % CAPACITY;
}*/

unsigned hashToken(Token t) {
	return (unsigned)t.type % CAPACITY;
}

ActionTable* initActionTable() {
	ActionTable* table = (ActionTable*)malloc(sizeof(ActionTable));
	if (!table) {
		printf(RED "Error: Failed to allocate memory for Action Table\n" RESET);
		exit(EXIT_FAILURE);
	}

	table->buckets = (ActionBucket**)malloc(CAPACITY * sizeof(ActionBucket*));
	if (!table->buckets) {
		printf(RED "Error: Failed to allocate memory for Action Buckets\n" RESET);
		exit(EXIT_FAILURE);
	}

	return table;
}

GotoTable* initGotoTable() {
	GotoTable* table = (GotoTable*)malloc(sizeof(GotoTable));
	if (!table) {
		printf(RED "Error: Failed to allocate memory for Goto Table\n" RESET);
		exit(EXIT_FAILURE);
	}

	table->buckets = (GotoBucket**)malloc(CAPACITY * sizeof(GotoBucket*));
	if (!table->buckets) {
		printf(RED "Error: Failed to allocate memory for Goto Buckets\n" RESET);
		exit(EXIT_FAILURE);
	}

	return table;
}

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

void createAction(ParsingTable* table, Token token, unsigned short state, void* func) {
	ActionBucket* bucket = (ActionBucket*)malloc(sizeof(ActionBucket));
	if (!bucket) {
		printf(RED "Error: Failed to allocate memory for Action Bucket\n" RESET);
		exit(EXIT_FAILURE);
	}

	bucket->function = func;
}

void createGoto(ParsingTable* table, Token token, unsigned short state, unsigned short newState) {
	GotoBucket* bucket = (GotoBucket*)malloc(sizeof(GotoBucket));
	if (!bucket) {
		printf(RED "Error: Failed to allocate memory for Action Bucket\n" RESET);
		exit(EXIT_FAILURE);
	}

	bucket->state = newState;
}

void* getAction(ParsingTable* table, Token token, unsigned short state) {

	return NULL;
}

unsigned short getGoto(ParsingTable* table, Token token, unsigned short state) {

	return 0;
}