#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"
#include "queue.h"
#include "bfs.h"

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
