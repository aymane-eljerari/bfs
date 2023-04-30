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
    return (q->rear == MAX_QUEUE_SIZE - 1);
}

int queueLength(Queue *q) {
    return (q->rear - q->front + 1);
}

void enqueue(Queue *q, int val) {
    if (isQueueFull(q)) {
        printf("Queue overflow\n");
        return;
    } else if (isQueueEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear++;
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
        q->front++;
    }
    return val;
}

void printQueue(Queue *q) {
    int i;
    printf("Queue: ");
    for (i = q->front; i <= q->rear; i++) {
        printf("%d ", q->data[i]);
    }
    printf("\n");
}
