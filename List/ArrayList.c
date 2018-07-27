#include "ArrayList.h"
#include <stdlib.h>

ArrayList *createArrayList(int initialCapacity) {
    ArrayList *list = (ArrayList *) malloc(sizeof(ArrayList));
    list->size = 0;
    list->capacity = initialCapacity;
    list->elements = (void **) malloc(sizeof(void *) * list->capacity);
    return list;
}

void freeArrayList(ArrayList *list) {
    free(list->elements);
    free(list);
}

void resizeArrayList(ArrayList *list) {
    list->capacity *= 2;
    void **newElements = (void **) malloc(sizeof(void *) * list->capacity);
    for(int i = 0; i < list->size; i++) {
        newElements[i] = list->elements[i];
    }
    free(list->elements);
    list->elements = newElements;
}

void addToArrayListAtIndex(ArrayList *list, int idx, void *element) {
    if(list->size >= list->capacity) {
        resizeArrayList(list);
    }
    for(int i = list->size; i > idx; i--) {
        list->elements[i] = list->elements[i - 1];
    }
    list->elements[idx] = element;
    list->size++;
}

void addToArrayListEnd(ArrayList *list, void *element) {
    if(list->size >= list->capacity) {
        resizeArrayList(list);
    }
    list->elements[list->size++] = element;
}

//return element
void *removeFromArrayListAtIndex(ArrayList *list, int idx) {
    if(idx < 0 || idx >= list->size) {
        return NULL;
    }
    void *removed = list->elements[idx];
    for(int i = idx; i < list->size; i++) {
        list->elements[i] = list->elements[i + 1];
    }
    list->size--;
    return removed;
}

bool removeElementFromArrayList(ArrayList *list, void *element) {
    for(int i = 0; i < list->size; i++) {
        if(list->elements[i] == element) {
            removeFromArrayListAtIndex(list, i);
            return true;
        }
    }
    return false;
}

//return old element
void *setArrayListElement(ArrayList *list, int idx, void *element) {
    void *old = list->elements[idx];
    list->elements[idx] = element;
    return old;
}

void *getArrayListElement(ArrayList *list, int idx) {
    if(0 <= idx && idx < list->size) {
        return list->elements[idx];
    }
    return NULL;
}

int arrayListSize(ArrayList *list) {
    return list->size;
}

#ifdef TEST_ARRAY_LIST
#include <assert.h>
int main(){
    ArrayList *list = createArrayList(2);
    assert(2 == list->capacity);
    assert(0 == list->size);
    
    int *a, *b, *c;
    double *d, *e, *f;
    char *x, *y;

    a = (int *) malloc(sizeof(int));
    b = (int *) malloc(sizeof(int));
    c = (int *) malloc(sizeof(int));

    d = (double *) malloc(sizeof(double));
    e = (double *) malloc(sizeof(double));
    f = (double *) malloc(sizeof(double));

    x = (char *) malloc(sizeof(char));
    y = (char *) malloc(sizeof(char));
    

    *a = 1;
    *b = 2;
    *c = 3;

    *d = 4.0;
    *e = 5.0;
    *f = 6.0;

    *x = 'a';
    *y = 'b';

    addToArrayListEnd(list, (void *) a);
    assert(2 == list->capacity);
    assert(1 == list->size);
    assert(1 == *( (int *) getArrayListElement(list, 0)));

    addToArrayListEnd(list, (void *) d);
    assert(2 == list->capacity);
    assert(2 == list->size);
    assert(1 == *( (int *) getArrayListElement(list, 0)));
    assert(4 == (int) *( (double *) getArrayListElement(list, 1)));

    addToArrayListAtIndex(list, 1, (void *) x);
    assert(4 == list->capacity);
    assert(3 == arrayListSize(list));
    assert(1 == *( (int *) getArrayListElement(list, 0)));
    assert('a' == *( (char *) getArrayListElement(list, 1)));
    assert(4 == (int) *( (double *) getArrayListElement(list, 2)));

    addToArrayListAtIndex(list, 0, (void *) b);
    assert(4 == arrayListSize(list));
    assert(2 == *( (int *) getArrayListElement(list, 0)));
    assert(1 == *( (int *) getArrayListElement(list, 1)));
    assert('a' == *( (char *) getArrayListElement(list, 2)));
    assert(4 == (int) *( (double *) getArrayListElement(list, 3)));

    addToArrayListAtIndex(list, 2, (void *) e);
    assert(8 == list->capacity);
    assert(5 == arrayListSize(list));
    assert(2 == *( (int *) getArrayListElement(list, 0)));
    assert(1 == *( (int *) getArrayListElement(list, 1)));
    assert(5 == (int) *( (double *) getArrayListElement(list, 2)));
    assert('a' == *( (char *) getArrayListElement(list, 3)));
    assert(4 == (int) *( (double *) getArrayListElement(list, 4)));

    assert(getArrayListElement(list, -1) == NULL);

    setArrayListElement(list, 2, f);
    assert(5 == arrayListSize(list));
    assert(6 == (int) *( (double *) getArrayListElement(list, 2)));

    assert(1 == *( (int *) removeFromArrayListAtIndex(list, 1)));
    assert(4 == arrayListSize(list));
    assert(2 == *( (int *) getArrayListElement(list, 0)));
    assert(6 == (int) *( (double *) getArrayListElement(list, 1)));
    assert('a' == *( (char *) getArrayListElement(list, 2)));
    assert(4 == (int) *( (double *) getArrayListElement(list, 3)));

    assert(removeElementFromArrayList(list, d));
    assert(3 == arrayListSize(list));
    assert(2 == *( (int *) getArrayListElement(list, 0)));
    assert(6 == (int) *( (double *) getArrayListElement(list, 1)));
    assert('a' == *( (char *) getArrayListElement(list, 2)));

    assert(!removeElementFromArrayList(list, c));

    freeArrayList(list);
    free(a);
    free(b);
    free(d);
    free(e);
    free(f);
    free(x);
    free(y);
    return 0;
}
#endif