#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "constants.h"

typedef struct {
    int front, rear;
    int data[MAX_QUEUE_SIZE];
} Queue;


void initQueue(Queue *q);
bool isQueueEmpty(Queue *q);
bool isQueueFull(Queue *q);
int queueLength(Queue *q);
void enqueue(Queue *q, int val);
int dequeue(Queue *q);

#endif
