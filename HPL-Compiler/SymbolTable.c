#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned hashSymbol(const char* key) {
	unsigned int hash = 5381;
	int c;
	
	while ((c = *key++))
		hash = ((hash << 5) + hash) + c; // hash * 33 + c

	return hash % CAPACITY;
}

SymbolTable* initSymbolTable() {
	SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
	if (!table) {
		printf(RED "Error: Failed to allocate Symbol Table\n" RESET);
		exit(EXIT_FAILURE);
	}

	table->capacity = CAPACITY;
	table->buckets = (SymbolEntry**)calloc(CAPACITY, sizeof(SymbolEntry*));
	if (!table->buckets) {
		printf(RED "Error: Failed to allocate Buckets for symbol table\n" RESET);
		free(table);
		exit(EXIT_FAILURE);
	}

	return table;
}

void putSymbol(SymbolTable* table, char* key, SymbolData data) {
	if (!key)
		return;

	int idx = hashSymbol(key);
	SymbolEntry* newNode = (SymbolEntry*)malloc(sizeof(SymbolEntry));
	if (!newNode) {
		printf(RED "Error: Failed to allocate memory for Symbol bucket\n" RESET);
		exit(EXIT_FAILURE);
	}

	newNode->key = _strdup(key);
	newNode->data = data;

	newNode->next = table->buckets[idx];
	table->buckets[idx] = newNode;
}

void removeSymbol(SymbolTable* table, const char* key) {
	if (!table || !key) return;

	unsigned int idx = hashSymbol(key);
	SymbolEntry** curr = &table->buckets[idx];
	SymbolEntry* temp;

	while (*curr) {
		if (strcmp((*curr)->key, key) == 0) {
			temp = *curr;
			*curr = (*curr)->next;

			free(temp->key);
			free(temp);
			return;
		}
		curr = &(*curr)->next;
	}
}

SymbolData getSymbol(SymbolTable* table, char* key) {
	SymbolData empty = { 0 };
	if (!table || !key) {
		return empty;
	}

	unsigned int idx = hashSymbol(key);
	SymbolEntry* node = table->buckets[idx];

	while (node) {
		if (strcmp(node->key, key) == 0) {
			return node->data;
		}
		node = node->next;
	}

	return empty;
}

void freeSymbolTable(SymbolTable* table) {
	if (!table) return;

	SymbolEntry* node, *next;
	int i;

	for (i = 0; i < table->capacity; i++) {
		node = table->buckets[i];

		while (node) {
			next = node->next;

			free(node->key);
			free(node);

			node = next;
		}
	}

	free(table->buckets);
	free(table);
}