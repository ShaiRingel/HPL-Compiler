#pragma once
#include "Global.h"

static unsigned hashNumber(unsigned num, int capacity) {
    return (num * 2654435761u) % capacity;
}

static unsigned hashChar(char c, int capacity) {
    return ((unsigned char)c * 31u) % capacity;
}