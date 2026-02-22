#include "GotoTable.h" 
#include <stdlib.h>
#include <stdio.h>

static unsigned hashState(unsigned short state) {
    return (unsigned)state % STATES_CAPACITY;
}

static unsigned hashToken(Token token) {
    return (unsigned)token.type % GRAMMAR_CAPACITY;
}

GotoHashMap* createGotoHashMap() {
    GotoHashMap* map = (GotoHashMap*)malloc(sizeof(GotoHashMap));
    if (!map) {
        fprintf(stderr, RED "Error: Failed to allocate memory for Goto map\n" RESET);
        exit(EXIT_FAILURE);
    }

    map->capacity = STATES_CAPACITY;
    map->buckets = (ParserGoto**)calloc(STATES_CAPACITY, sizeof(ParserGoto*));
    if (!map->buckets) {
        fprintf(stderr, RED "Error: Failed to allocate Goto buckets\n" RESET);
        exit(EXIT_FAILURE);
    }

    return map;
}

GotoTableEntry* createGotoEntry(Token token) {
    GotoTableEntry* entry = (GotoTableEntry*)malloc(sizeof(GotoTableEntry));
    if (!entry) {
        fprintf(stderr, RED "Error: Failed to allocate Goto entry\n" RESET);
        exit(EXIT_FAILURE);
    }

    entry->token = token;
    entry->gotos = createGotoHashMap();
    entry->next = NULL;

    return entry;
}

ParserGoto* findGoto(GotoHashMap* map, unsigned short state) {
    if (!map) return NULL;
    ParserGoto* gt;
    int idx;
    
    idx = hashState(state);
    gt = map->buckets[idx];

    while (gt) {
        if (gt->state == state) return gt;
        gt = gt->next;
    }

    return NULL;
}

GotoTableEntry* findGotoEntry(GotoTable* table, Token token) {
    GotoTableEntry* entry;
    int idx;

    idx = hashToken(token);
    entry = table->buckets[idx];

    while (entry) {
        if (entry->token.type == token.type) return entry;
        entry = entry->next;
    }

    return NULL;
}

GotoTable* initGotoTable() {
    GotoTable* table = (GotoTable*)malloc(sizeof(GotoTable));
    if (!table) {
        printf(RED "Error: Failed to allocate Goto Table\n" RESET);
        exit(EXIT_FAILURE);
    }

    table->capacity = GRAMMAR_CAPACITY;
    table->buckets = (GotoTableEntry**)calloc(GRAMMAR_CAPACITY, sizeof(GotoTableEntry*));
    if (!table->buckets) {
        printf(RED "Error: Failed to allocate Goto Table Buckets\n" RESET);
        exit(EXIT_FAILURE);
    }

    return table;
}

GotoTableEntry* getOrCreateGotoEntry(GotoTable* table, Token token) {
    GotoTableEntry* entry;
    int idx;
    
    entry = findGotoEntry(table, token);
    if (entry) return entry;

    idx = hashToken(token);
    entry = createGotoEntry(token);

    entry->next = table->buckets[idx];
    table->buckets[idx] = entry;

    return entry;
}

void createGoto(GotoTable* table, Token token, unsigned short state, unsigned short newState) {
    GotoTableEntry* entry;
    ParserGoto* gt;
    int idx;

    idx = hashState(state);
    entry = getOrCreateGotoEntry(table, token);
    ParserGoto* current = entry->gotos->buckets[idx];
    while (current) {
        if (current->state == state) {
            current->newState = newState;
            return;
        }
        current = current->next;
    }

    gt = (ParserGoto*)malloc(sizeof(ParserGoto));
    if (!gt) {
        fprintf(stderr, RED "Error: Failed to allocate ParserGoto node\n" RESET);
        exit(EXIT_FAILURE);
    }

    gt->state = state;
    gt->newState = newState;

    gt->next = entry->gotos->buckets[idx];
    entry->gotos->buckets[idx] = gt;
}

unsigned short getGoto(GotoTable* table, Token token, unsigned short state) {
    GotoTableEntry* entry;
    ParserGoto* gt;

    entry = findGotoEntry(table, token);
    if (!entry) return 0;

    gt = findGoto(entry->gotos, state);
    return gt ? gt->newState : 0;
}

void freeGotoHashMap(GotoHashMap* map) {
    int i;

    if (!map) return;
    
    for (i = 0; i < map->capacity; i++) {
        ParserGoto* gt = map->buckets[i];
        while (gt) {
            ParserGoto* temp = gt;
            gt = gt->next;
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}

void freeGotoTable(GotoTable* table) {
    int i;

    if (!table) return;
    
    for (i = 0; i < table->capacity; i++) {
        GotoTableEntry* entry = table->buckets[i];
        while (entry) {
            GotoTableEntry* temp = entry;
            entry = entry->next;
            freeGotoHashMap(temp->gotos);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}