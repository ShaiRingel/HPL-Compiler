#pragma once
#include <stdlib.h>

typedef struct {
	char* dataType;
	char* scope;
} listItemType;


typedef struct nodeType {
	char* key;
	listItemType value;
	struct nodeType *next;
} Node;


void setNode(Node *node, char* key, listItemType value);