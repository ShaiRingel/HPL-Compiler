#include "LinkedList.h"

void setNode(Node* node, char* key, listItemType value) {
	node->key = key;
	node->value = value;
	node->next = NULL;
}