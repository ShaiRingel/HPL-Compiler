#include "TransitionTable.h"
#include "Global.h"
#include <stdlib.h>

unsigned hashState(unsigned short state) {
    return (unsigned)state % STATE_CAPACITY;
}

unsigned hashChar(char c) {
    return (unsigned)c % CHAR_CAPACITY;
}

StateBucket* findStateBucket(const TransitionTable* table, unsigned short state) {
    StateBucket* current;
    unsigned int idx;

    idx = hashState(state);
    current = table->buckets[idx];

    while (current) {
        if (current->key == state) return current;
        current = current->next;
    }
    return NULL;
}

CharBucket* findCharBucket(const CharMap* map, char symbol) {
    CharBucket* current;
    unsigned int idx;

    idx = hashChar(symbol);
    current = map->buckets[idx];

    while (current) {
        if (current->key == symbol) return current;
        current = current->next;
    }
    return NULL;
}

CharMap* createCharMap() {
    CharMap* map;

    map = (CharMap*)malloc(sizeof(CharMap));
    if (!map) exit(EXIT_FAILURE);

    map->capacity = CHAR_CAPACITY;
    map->buckets = (CharBucket**)calloc(CHAR_CAPACITY, sizeof(CharBucket*));
    if (!map->buckets) exit(EXIT_FAILURE);

    return map;
}

StateBucket* createStateBucket(int state) {
    StateBucket* sb;

    sb = (StateBucket*)malloc(sizeof(StateBucket));
    if (!sb) exit(EXIT_FAILURE);

    sb->key = state;
    sb->value = createCharMap();
    sb->token = 0;
    sb->next = NULL;
    return sb;
}

StateBucket* getOrCreateStateBucket(TransitionTable* table, unsigned short state) {
    StateBucket* sBucket;
    unsigned int idx;

    sBucket = findStateBucket(table, state);
    if (sBucket) return sBucket;

    idx = hashState(state);
    sBucket = createStateBucket(state);
    sBucket->next = table->buckets[idx];
    table->buckets[idx] = sBucket;
    return sBucket;
}

TransitionTable* initTransitionTable() {
    TransitionTable* table;

    table = (TransitionTable*)malloc(sizeof(TransitionTable));
    if (!table) exit(EXIT_FAILURE);

    table->stateCounter = 0;
    table->capacity = STATE_CAPACITY;
    table->buckets = (StateBucket**)calloc(STATE_CAPACITY, sizeof(StateBucket*));
    if (!table->buckets) exit(EXIT_FAILURE);

    getOrCreateStateBucket(table, 0);

    return table;
}

void insertTransition(TransitionTable* table, unsigned short state, char symbol, unsigned short newState) {
    StateBucket* sBucket;
    CharBucket* cBucket;
    unsigned int cIdx;
    CharMap* map;

    sBucket = getOrCreateStateBucket(table, state);
    map = sBucket->value;

    cBucket = (CharBucket*)malloc(sizeof(CharBucket));
    if (!cBucket) exit(EXIT_FAILURE);

    cIdx = hashChar(symbol);
    cBucket->key = symbol;
    cBucket->value = newState;
    cBucket->next = map->buckets[cIdx];
    map->buckets[cIdx] = cBucket;
}

unsigned short getState(const TransitionTable* table, unsigned short state, char symbol) {
    StateBucket* sBucket;
    CharBucket* cBucket;

    sBucket = findStateBucket(table, state);
    if (!sBucket) return 0;

    cBucket = findCharBucket(sBucket->value, symbol);
    return (cBucket) ? (int)cBucket->value : 0;
}

void setToken(TransitionTable* table, unsigned short state, TokenType token) {
    StateBucket* sBucket;

    sBucket = getOrCreateStateBucket(table, state);
    sBucket->token = token;
}

TokenType getTokenType(const TransitionTable* table, unsigned short state) {
    StateBucket* sBucket;

    sBucket = findStateBucket(table, state);
    return (sBucket) ? sBucket->token : 0;
}

void destroyCharMap(CharMap* map) {
    int i;
    CharBucket *current, *temp;

    if (!map) return;

    for (i = 0; i < map->capacity; i++) {
        current = map->buckets[i];
        while (current) {
            temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(map->buckets);
    free(map);
}

void destroyTransitionTable(TransitionTable* table) {
    int i;
    StateBucket *current, *temp;

    if (!table) return;

    for (i = 0; i < table->capacity; i++) {
        current = table->buckets[i];
        while (current) {
            temp = current;
            current = current->next;
            destroyCharMap(temp->value);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}