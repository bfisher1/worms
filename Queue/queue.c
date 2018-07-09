/**
    @file queue.c
    @author Ben Fisher

    Implementation of a queue. The data structure is a linked,
    circular queue. It is made up of a series of linked QueueNodes.
    Each QueueNode contains a pointer to the next node in the queue.
    The queue itself contains a reference to a single
    node, the back node. This node's next field points to the front
    node. The rest of the nodes are linked in normal order, from front
    to back. Since each node is linked, they form a circle. All
    operations on the queue run in constant time.
*/
#include "queue.h"
#include <stdlib.h>

Queue *makeQueue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->back = NULL;
    queue->numElements = 0;
    return queue;
}

void enqueue(Queue *queue, void *element){
    if(queue->back == NULL){
        queue->back = (QueueNode *) malloc(sizeof(QueueNode));
        queue->back->element = element;
        queue->back->next = queue->back;
        queue->numElements++;
    }
    else {
        QueueNode *front = queue->back->next;
        QueueNode *newBack = (QueueNode *) malloc(sizeof(QueueNode));
        newBack->element = element;
        newBack->next = front;
        queue->back->next = newBack;
        queue->back = newBack;
        queue->numElements++;
    }
}

void *peekFront(Queue *queue) {
    if(queue->back == NULL){
        return NULL;
    }
    return queue->back->next->element;
}

void *peekBack(Queue *queue) {
    if(queue->back == NULL){
        return NULL;
    }
    return queue->back->element;
}

//does not free dequeued elements
void *dequeue(Queue *queue){
    if(queue->back == NULL){
        return NULL;
    }
    if(queue->numElements == 1){
        void *element = queue->back->element;
        free(queue->back);
        queue->back = NULL;
        queue->numElements--;
        return element;
    } else {
        QueueNode *dequeued = queue->back->next;
        queue->back->next = queue->back->next->next;
        void *element = dequeued->element;
        free(dequeued);
        queue->numElements--;
        return element;
    }
}

bool isQueueEmpty(Queue *queue) {
    return queue->numElements == 0;
}

int queueSize(Queue *queue) {
    return queue->numElements;
}

void freeQueue(Queue *queue){
    if(queue->back != NULL) {
        QueueNode *node = queue->back;
        QueueNode *next;
        for(int i = 0; i < queue->numElements; i++){
            next = node->next;
            free(node);
            node = next;
        }
    }
    free(queue);
}