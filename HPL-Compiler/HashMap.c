#include "HashMap.h"

void initHashMap(HashMap* mp) {
    int i;
    mp->capacity = 100;
    mp->numOfElements = 0;

    mp->arr = (Node**)malloc(sizeof(Node*) * mp->capacity);
    if (!mp->arr) {
        perror("Malloc Failed");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < mp->capacity; i++)
        mp->arr[i] = NULL;
    
}

int hashFunction(HashMap* mp, char* key) {
    int bucketIndex;
    int sum = 0, factor = 31;

    for (int i = 0; i < strlen(key); i++) {
        sum = ((sum % mp->capacity)
            + (((int)key[i]) * factor) % mp->capacity)
            % mp->capacity;

        factor = ((factor % SHRT_MAX)
            * (31 % SHRT_MAX))
            % SHRT_MAX;
    }

    bucketIndex = sum;
    return bucketIndex;
}

void insert(HashMap* mp, char* key, listItemType value) {
    int bucketIndex = hashFunction(mp, key);
    Node* newNode = (struct node*)malloc(sizeof(Node));

    setNode(newNode, key, value);

    if (mp->arr[bucketIndex] == NULL) {
        mp->arr[bucketIndex] = newNode;
    }
    else {
        newNode->next = mp->arr[bucketIndex];
        mp->arr[bucketIndex] = newNode;
    }

}

void deletion(HashMap *mp, char* key) {
    int bucketIndex = hashFunction(mp, key);

    Node* prevNode = NULL;

    Node* currNode = mp->arr[bucketIndex];

    while (currNode != NULL) {
        if (strcmp(key, currNode->key) == 0) {
            if (currNode == mp->arr[bucketIndex]) {
                mp->arr[bucketIndex] = currNode->next;
            }

            else {
                prevNode->next = currNode->next;
            }
            free(currNode);
            break;
        }

        prevNode = currNode;
        currNode = currNode->next;
    }
    return;
}

listItemType get(HashMap* mp, char* key) {
    int bucketIndex = hashFunction(mp, key);

    Node* bucketHead = mp->arr[bucketIndex];
    while (bucketHead != NULL) {
        if (!strcmp(bucketHead->key, key)) {
            return bucketHead->value;
        }
        bucketHead = bucketHead->next;
    }
    listItemType l;
    l.dataType = 0;

    return l;
}
