/**
    @file queue.h
    @author Ben Fisher

    Declares functions and structs related to the Queue ADT.
    A queue contains a collection of elements that are added and
    removed the same way people enter and exit a shopping line.
    That is, elements are added to the back and removed from the
    front. This queue is generic in the sense that it can store
    any item. It keeps track of pointers to its elements, so this
    qeueue will only work with elements on the heap, statically
    allocated, or in a function that has not been deleted from the
    stack.

    When this queue dequeues an element, that element is not freed
    autmoatically. The user must take care of freeing these elements
    themselves.
*/
#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>

/**
    Node in the queue, contains a pointer to a single element.
*/
typedef struct QueueNodeTag {
    void *element;
    struct QueueNodeTag *next;
} QueueNode;

/**
    Structure of the queue. Contains a pointer to the back node.
*/
typedef struct {
    QueueNode *back;
    int numElements;
} Queue;

/**
    Creates an empty queue.

    @return new queue
*/
Queue *makeQueue();

/**
    Adds an element to the back of the queue.

    @param queue the queue having an element added
    @param element the element being added
*/
void enqueue(Queue *queue, void *element);

/**
    Returns the element at the front of the queue.

    @param queue the queue being peeked
    @return the element at the front of the queue
*/
void *peekFront(Queue *queue);

/**
    Returns the element at the back of the queue.

    @param queue the queue being peeked
    @return the element at the back of the queue
*/
void *peekBack(Queue *queue);

/**
    Removes an element from the front of the queue.

    @param queue the queue having its front removed
    @return the element at the front of the queue
*/
void *dequeue(Queue *queue);

/**
    Determines if the queue is empty.

    @param queue the queue that may be empty
    @return true if the queue is empty, false if it has elements
*/
bool isQueueEmpty(Queue *queue);

/**
    Returns the queue's size.

    @param queue the queue being measured
    @return the number of elements in the queue
*/
int queueSize(Queue *queue);

/**
    Frees a queue from memory.
    
    @param queue the queue being freed
*/
void freeQueue(Queue *queue);
#endif /* QUEUE_H */