#include "TransitionTable.h"
#include "Global.h"
#include <stdlib.h>

static unsigned hashState(unsigned short state) {
    return (state * 2654435761u) % STATE_CAPACITY;
}

static unsigned hashChar(char c, int capacity) {
    return ((unsigned char)c * 31u) % capacity;
}

StateBucket* findStateBucket(const TransitionTable* table, unsigned short state) {
    StateBucket* current;
    unsigned int idx;

    idx = hashState(state);
    current = table->buckets[idx];

    while (current && current->key != state) {
        current = current->next;
    }
    return current;
}

CharBucket* findCharBucket(const CharMap* map, char symbol) {
    CharBucket* current;
    unsigned int idx;

    idx = hashChar(symbol, map->capacity);
    current = map->buckets[idx];

    while (current && current->key != symbol)
        current = current->next;
    
    return current;
}

CharMap* createCharMap() {
    CharMap* map;

    map = (CharMap*)malloc(sizeof(CharMap));
    if (!map) exit(EXIT_FAILURE);

    map->capacity = INITIAL_CHAR_CAPACITY;
    map->buckets = (CharBucket**)calloc(INITIAL_CHAR_CAPACITY, sizeof(CharBucket*));
    if (!map->buckets) exit(EXIT_FAILURE);

    return map;
}

void expandCharMap(CharMap* map) {
    unsigned i, newIdx;
    CharBucket** oldBuckets = map->buckets;
    CharBucket* current, *next;

    map->capacity = EXPANDED_CHAR_CAPACITY;
    map->buckets = (CharBucket**)calloc(EXPANDED_CHAR_CAPACITY, sizeof(CharBucket*));
    if (!map->buckets) exit(EXIT_FAILURE);

    for (i = 0; i < INITIAL_CHAR_CAPACITY; i++) {
        current = oldBuckets[i];
        while (current) {
            CharBucket* next = current->next;

            newIdx = hashChar(current->key, map->capacity);
            current->next = map->buckets[newIdx];
            map->buckets[newIdx] = current;

            current = next;
        }
    }
    free(oldBuckets);
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

    if (*map->buckets && map->capacity == INITIAL_CHAR_CAPACITY) {
        expandCharMap(map);
    }

    cBucket = (CharBucket*)malloc(sizeof(CharBucket));
    if (!cBucket) exit(EXIT_FAILURE);

    cIdx = hashChar(symbol, map->capacity);
    cBucket->key = symbol;
    cBucket->value = newState;
    cBucket->next = map->buckets[cIdx];
    map->buckets[cIdx] = cBucket;
}
unsigned short getState(const TransitionTable* table, unsigned short state, char symbol) {
    StateBucket* sBucket;
    CharBucket* cBucket;

    sBucket = findStateBucket(table, state);
    if (!sBucket) return STATE_ERROR;

    cBucket = findCharBucket(sBucket->value, symbol);
    return (cBucket) ? (int)cBucket->value : STATE_ERROR;
}

void setToken(TransitionTable* table, unsigned short state, TokenType token) {
    StateBucket* sBucket;

    sBucket = getOrCreateStateBucket(table, state);
    sBucket->token = token;
}

TokenType getTokenType(const TransitionTable* table, unsigned short state) {
    StateBucket* sBucket;

    sBucket = findStateBucket(table, state);
    return (sBucket) ? sBucket->token : TOKEN_IDENT;
}

void freeCharMap(CharMap* map) {
    CharBucket *current, *temp;
    int i;

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

void freeTransitionTable(TransitionTable* table) {
    StateBucket *current, *temp;
    int i;

    if (!table) return;

    for (i = 0; i < table->capacity; i++) {
        current = table->buckets[i];
        while (current) {
            temp = current;
            current = current->next;
            freeCharMap(temp->value);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}