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

TransitionTable* initTransitionTable() {

	return NULL;
}

void destroyTable(TransitionTable* table) {

}

void insertTransition(TransitionTable* table, int state, char symbol) {

}

short getState(const TransitionTable* table, int state, char symbol) {

	return 0;
}