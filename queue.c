#include <stdbool.h>
#include <stdio.h>

#include "include/constants.h"
#include "include/queue.h"



/* Queue operations */
void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

bool isQueueEmpty(Queue *q) {
    return (q->front == -1 && q->rear == -1);
}

bool isQueueFull(Queue *q) {
    return ((q->rear+1)%MAX_QUEUE_SIZE == q->front);
}

int queueLength(Queue *q) {
    return (q->rear - q->front + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
}

void enqueue(Queue *q, int val) {
    if (isQueueFull(q)) {
        printf("Queue overflow\n");
        return;
    } else if (isQueueEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear+1)%MAX_QUEUE_SIZE;
    }
    q->data[q->rear] = val;
}

int dequeue(Queue *q) {
    int val;
    if (isQueueEmpty(q)) {
        printf("Queue underflow\n");
        return -1;
    } else if (q->front == q->rear) {
        val = q->data[q->front];
        q->front = q->rear = -1;
    } else {
        val = q->data[q->front];
        q->front = (q->front+1)%MAX_QUEUE_SIZE;
    }
    return val;
}
