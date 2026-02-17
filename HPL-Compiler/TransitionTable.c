#include "TransitionTable.h"
#include <stdio.h>
#include <stdlib.h>

unsigned hash_int(int x) {
	x ^= x >> 16;
	x *= 0x7feb352d;
	x ^= x >> 15;
	x *= 0x846ca68b;
	x ^= x >> 16;
	return (unsigned)x;
}

unsigned hash_char(char c) {
	return (unsigned)c * 2654435761u;
}

CharMap* createCharMap() {
	CharMap* map = (CharMap*)malloc(sizeof(CharMap));
	if (!map) {
		printf(RED "Error: Failed to allocate memory for CharMap\n" RESET);
		exit(EXIT_FAILURE);
	}

	map->capacity = CHAR_CAPACITY;
	map->buckets = (CharBucket*)calloc(CHAR_CAPACITY, sizeof(CharMap));
	if (!map->buckets) {
		printf(RED "Error: Failed to allocate memory for CharBuckets\n" RESET);
		exit(EXIT_FAILURE);
	}

	return map;
}

void freeCharMap(CharMap *map) {
	int i;
	
	for (i = 0; i < map->capacity; i++)
		free(&map->buckets[i]);

	free(map);
}

TransitionTable* initTransitionTable() {
	TransitionTable* table = (TransitionTable*)malloc(sizeof(TransitionTable));
	if (!table) {
		printf(RED "Error: Failed to allocate memory for TransitionTable\n" RESET);
		exit(EXIT_FAILURE);
	}

	table->capacity = STATE_CAPACITY;
	table->buckets = (StateBucket*)calloc(STATE_CAPACITY, sizeof(StateBucket));
	if (!table->buckets) {
		printf(RED "Error: Failed to allocate memory for StateBuckets\n" RESET);
		exit(EXIT_FAILURE);
	}
	
	return table;
}

void destroyTable(TransitionTable* table) {
	int i;

	for (i = 0; i < table->capacity; i++)
		free(&table->buckets[i]);

	free(table);
}

void insertTransition(TransitionTable* table, short state, char symbol) {

}

short getState(const TransitionTable* table, short state, char symbol) {

	return 0;
}