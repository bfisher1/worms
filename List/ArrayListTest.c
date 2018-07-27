#include "ArrayList.h"
#include <stdlib.h>
#include <assert.h>
int main(){
    ArrayList *list = createArrayList(2);
    assert(2 == list->capacity);
    assert(0 == list->size);
    
    int *a, *b, *c;
    double *d, *e, *f;
    char *x, *y, *z;

    a = (int *) malloc(sizeof(int));
    b = (int *) malloc(sizeof(int));
    c = (int *) malloc(sizeof(int));

    d = (int *) malloc(sizeof(double));
    e = (int *) malloc(sizeof(double));
    f = (int *) malloc(sizeof(double));

    x = (int *) malloc(sizeof(char));
    y = (int *) malloc(sizeof(char));
    z = (int *) malloc(sizeof(char));
    
    

    *a = 1;
    *b = 2;
    *c = 3;

    *d = 4.0;
    *e = 5.0;
    *f = 6.0;

    *x = 'a';
    *y = 'b';
    *z = 'c';

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

    

    freeArrayList(list);
    free(a);
    free(b);
    free(c);
    free(d);
    free(e);
    free(f);
    free(x);
    free(y);
    free(z);
    return 0;
}