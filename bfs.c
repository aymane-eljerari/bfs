#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "include/graph.h"
#include "include/queue.h"


/* Sequential BFS */
bool bfs(Graph *g, int startVertex, int targetVertex) {
    bool visited[g->vertexCount];
    int i, j;
    for (i = 0; i < g->vertexCount; i++) {
        visited[i] = false;
    }

    Queue q;
    initQueue(&q);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    while (!isQueueEmpty(&q)) {
        int currVertex = dequeue(&q);

        if (currVertex == targetVertex) {
            return true;
        }

        for (j = 0; j < g->vertexCount; j++) {
            if (g->adjMatrix[currVertex][j] == 1 && !visited[j]) {
                visited[j] = true;
                enqueue(&q, j);
            }
        }
    }

    return false;
}
