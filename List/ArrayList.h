#ifndef ARRAY_LIST
#define ARRAY_LIST
#include <stdbool.h>

typedef struct {
    void **elements;
    int size;
    int capacity;
} ArrayList;

ArrayList *createArrayList(int initialCapacity);

void freeArrayList(ArrayList *list);

void addToArrayListAtIndex(ArrayList *list, int idx, void *element);

void addToArrayListEnd(ArrayList *list, void *element);

//return element
void *removeFromArrayListAtIndex(ArrayList *list, int idx);

bool removeElementFromArrayList(ArrayList *list, void *element);

//return old element
void *setArrayListElement(ArrayList *list, int idx, void *element);

void *getArrayListElement(ArrayList *list, int idx);

int arrayListSize(ArrayList *list);

#endif /* ARRAY_LIST */
