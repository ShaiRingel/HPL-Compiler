#include "ActionTable.h"
#include <stdlib.h>
#include <stdio.h>

static unsigned hashState(unsigned short state)
{
    return (unsigned)state % STATES_CAPACITY;
}

static unsigned hashToken(Token token) {
    return (unsigned)token.type % GRAMMAR_CAPACITY;
}

ActionHashMap* createActionHashMap(void)
{
    ActionHashMap* map;

    map = (ActionHashMap*)malloc(sizeof(ActionHashMap));
    if (!map) {
        fprintf(stderr, RED "Error: Failed to allocate ActionHashMap\n" RESET);
        exit(EXIT_FAILURE);
    }

    map->capacity = STATES_CAPACITY;
    map->buckets = (ParserAction**)calloc(STATES_CAPACITY,
        sizeof(ParserAction*));
    if (!map->buckets) {
        fprintf(stderr, RED "Error: Failed to allocate ActionHashMap buckets\n" RESET);
        exit(EXIT_FAILURE);
    }

    return map;
}

ActionTableEntry* createActionEntry(Token token)
{
    ActionTableEntry* entry;

    entry = (ActionTableEntry*)malloc(sizeof(ActionTableEntry));
    if (!entry) {
        fprintf(stderr, RED "Error: Failed to allocate ActionTableEntry\n" RESET);
        exit(EXIT_FAILURE);
    }

    entry->token = token;
    entry->actions = createActionHashMap();
    entry->next = NULL;

    return entry;
}

ParserAction* findActionInMap(ActionHashMap* map,
    unsigned short state)
{
    ParserAction* pa;
    unsigned idx;

    if (!map)
        return NULL;

    idx = hashState(state);
    pa = map->buckets[idx];

    while (pa) {
        if (pa->state == state)
            return pa;
        pa = pa->next;
    }

    return NULL;
}

ActionTableEntry* findActionEntry(ActionTable* table,
    Token token)
{
    ActionTableEntry* entry;
    unsigned idx;

    if (!table)
        return NULL;

    idx = hashToken(token);
    entry = table->buckets[idx];

    while (entry) {
        if (entry->token.type == token.type)
            return entry;
        entry = entry->next;
    }

    return NULL;
}

ActionTable* initActionTable(void)
{
    ActionTable* table;

    table = (ActionTable*)malloc(sizeof(ActionTable));
    if (!table) {
        fprintf(stderr, RED "Error: Failed to allocate ActionTable\n" RESET);
        exit(EXIT_FAILURE);
    }

    table->capacity = GRAMMAR_CAPACITY;
    table->buckets = (ActionTableEntry**)calloc(GRAMMAR_CAPACITY,
        sizeof(ActionTableEntry*));
    if (!table->buckets) {
        fprintf(stderr, RED "Error: Failed to allocate ActionTable buckets\n" RESET);
        exit(EXIT_FAILURE);
    }

    return table;
}

ActionTableEntry* getOrCreateActionEntry(ActionTable* table,
    Token token)
{
    ActionTableEntry* entry;
    unsigned idx;

    entry = findActionEntry(table, token);
    if (entry)
        return entry;

    idx = hashToken(token);
    entry = createActionEntry(token);

    entry->next = table->buckets[idx];
    table->buckets[idx] = entry;

    return entry;
}

void createAction(ActionTable* table,
    Token token,
    unsigned short state,
    ActionType type,
    ActionData data)
{
    ActionTableEntry* entry;
    ParserAction* current;
    ParserAction* pa;
    unsigned idx;

    if (!table)
        return;

    entry = getOrCreateActionEntry(table, token);
    idx = hashState(state);

    current = entry->actions->buckets[idx];
    while (current) {
        if (current->state == state) {
            current->type = type;
            current->data = data;
            return;
        }
        current = current->next;
    }

    pa = (ParserAction*)malloc(sizeof(ParserAction));
    if (!pa) {
        fprintf(stderr, RED "Error: Failed to allocate ParserAction\n" RESET);
        exit(EXIT_FAILURE);
    }

    pa->state = state;
    pa->type = type;
    pa->data = data;

    pa->next = entry->actions->buckets[idx];
    entry->actions->buckets[idx] = pa;
}

ParserAction* getAction(ActionTable* table,
    Token token,
    unsigned short state)
{
    ActionTableEntry* entry;

    if (!table)
        return NULL;

    entry = findActionEntry(table, token);
    if (!entry)
        return NULL;

    return findActionInMap(entry->actions, state);
}

void freeActionHashMap(ActionHashMap* map)
{
    int i;
    ParserAction* pa;
    ParserAction* temp;

    if (!map)
        return;

    for (i = 0; i < map->capacity; i++) {
        pa = map->buckets[i];
        while (pa) {
            temp = pa;
            pa = pa->next;
            free(temp);
        }
    }

    free(map->buckets);
    free(map);
}

void freeActionTable(ActionTable* table)
{
    int i;
    ActionTableEntry* entry;
    ActionTableEntry* temp;

    if (!table)
        return;

    for (i = 0; i < table->capacity; i++) {
        entry = table->buckets[i];
        while (entry) {
            temp = entry;
            entry = entry->next;
            freeActionHashMap(temp->actions);
            free(temp);
        }
    }

    free(table->buckets);
    free(table);
}