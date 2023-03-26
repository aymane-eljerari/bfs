#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_QUEUE_SIZE 100

/* Queue data structure */
typedef struct {
    int front, rear;
    int data[MAX_QUEUE_SIZE];
} Queue;

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

/* Graph data structure */
typedef struct {
    int vertexCount;
    int **adjMatrix;
} Graph;

/* Graph operations */
void initGraph(Graph *g, int vertexCount) {
    g->vertexCount = vertexCount;
    g->adjMatrix = (int**)malloc(vertexCount * sizeof(int*));
    for (int i = 0; i < vertexCount; i++) {
        g->adjMatrix[i] = (int*)calloc(vertexCount, sizeof(int));
    }
}

void addEdge(Graph *g, int src, int dest) {
    g->adjMatrix[src][dest] = 1;
    g->adjMatrix[dest][src] = 1;
}

/* Breadth-first search algorithm */
void bfs(Graph *g, int startVertex) {
    bool visited[g->vertexCount];
    for (int i = 0; i < g->vertexCount; i++) {
        visited[i] = false;
    }

    Queue q;
    initQueue(&q);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    while (!isQueueEmpty(&q)) {
        int currVertex = dequeue(&q);
        printf("%d ", currVertex);

        for (int i = 0; i < g->vertexCount; i++) {
            if (g->adjMatrix[currVertex][i] == 1 && !visited[i]) {
                visited[i] = true;
                enqueue(&q, i);
            }
        }
    }
}

/* Test the algorithm */
int main() {
    Graph g;
    int vertexCount = 6;
    initGraph(&g, vertexCount);

    addEdge(&g, 0, 1);
    addEdge(&g, 0, 2);
    addEdge(&g, 1, 3);
    addEdge(&g, 1, 4);
    addEdge(&g, 2, 4);
    addEdge(&g, 3, 4);
    addEdge(&g, 3, 5);

    printf("BFS Traversal: ");
    bfs(&g, 0);

    return 0;
}
