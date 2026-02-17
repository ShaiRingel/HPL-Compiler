#pragma once
#include "Global.h"

#define CAPACITY 100

typedef struct {
    char key;          // input symbol
    short value;       // next state
    Bool used;
} CharBucket;

typedef struct {
    CharBucket* buckets;   // array of buckets
    int capacity;          // total bucket count
} CharMap;

typedef struct {
    int key;           // state ID
    CharMap value;     // inner map
    Bool used;
} StateBucket;

typedef struct {
    StateBucket* buckets;  // array of buckets
    int capacity;          // total bucket count
} TransitionTable;

// Create a transition table with a given number of buckets
TransitionTable* initTransitionTable();

// Free all memory associated with the table
void destroyTable(TransitionTable* table);

// Add a transition: state & symbol --> new_state
void insertTransition(TransitionTable* table, int state, char symbol);

// Get next state, or -1 if no transition exists
short getState(const TransitionTable* table, int state, char symbol);
