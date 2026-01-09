#pragma once
#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int numOfElements, capacity;

    Node** arr;
} HashMap;

void initHashMap(HashMap *mp);

int hashFunction(HashMap* mp, char* key);

void insert(HashMap *hm, char *key, listItemType value);

void deletion(HashMap* mp, char* key);

listItemType get(HashMap *hm, char *key);