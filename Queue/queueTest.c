/**
    @file queueTest.c
    @author Ben Fisher

    Tests that the queue implementation is behaving properly
    as a generic queue. Runs through all operations related to
    a queue.
*/
#include "queue.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main() {
    int *a1 = (int *) malloc(sizeof(int));
    int *a2 = (int *) malloc(sizeof(int));
    int *a3 = (int *) malloc(sizeof(int));
    char *str = (char *) malloc(sizeof(char) * 10);
    sprintf(str, "kangaroo");
    char *ch = (char *) malloc(sizeof(char));
    *ch = 'x';
    *a1 = 10;
    *a2 = 32;
    *a3 = -7;
    Queue *queue = makeQueue();
    assert(isQueueEmpty(queue));
    enqueue(queue, (void *) a1);
    enqueue(queue, (void *) a2);
    enqueue(queue, (void *) a3);
    assert(!isQueueEmpty(queue));
    assert(3 == queueSize(queue));
    assert(-7 == *((int *) peekBack(queue)));
    assert(10 == *((int *) peekFront(queue)));
    assert(10 == *((int *) dequeue(queue)));
    assert(32 == *((int *) dequeue(queue)));
    assert(1 == queueSize(queue));
    assert(-7 == *((int *) dequeue(queue)));
    assert(0 == queueSize(queue));
    assert(isQueueEmpty(queue));

    *a1 = 97;
    *a3 = -9000;
    enqueue(queue, str);
    assert(!isQueueEmpty(queue));
    enqueue(queue, (void *) a3);
    enqueue(queue, ch);
    enqueue(queue, (void *) a1);

    assert(4 == queueSize(queue));
    assert(97 == *((int *) peekBack(queue)));
    assert('k' == ((char *) peekFront(queue))[0] );
    assert('a' == ((char *) peekFront(queue))[1] );
    assert('n' == ((char *) peekFront(queue))[2] );
    assert('k' == *((char *) dequeue(queue)));
    assert(-9000 == *((int *) dequeue(queue)));
    assert('x' == *((char *) dequeue(queue)));
    assert(97 == *((int *) dequeue(queue)));
    freeQueue(queue);
    queue = makeQueue();
    enqueue(queue, a1);
    enqueue(queue, a2);
    enqueue(queue, str);
    freeQueue(queue);
    queue = makeQueue();
    enqueue(queue, a1);
    enqueue(queue, a2);
    enqueue(queue, str);
    dequeue(queue);
    freeQueue(queue);
    

    printf("Queue Tests Passed\n");
    free(a1);
    free(a2);
    free(a3);
    free(str);
    free(ch);
}
