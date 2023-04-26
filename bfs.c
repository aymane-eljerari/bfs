#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#include "graph.h"
#include "queue.h"
#include "bfs.h"

/* Sequential BFS */
bool bfsSearch(Graph *g, int startVertex, int targetVertex) {
    bool visited[g->vertexCount] = {false};

    Queue q;
    initQueue(&q);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    while (!isQueueEmpty(&q)) {
        int currVertex = dequeue(&q);

        if (currVertex == targetVertex) {
            return true;
        }

        for (int i = 0; i < g->vertexCount; i++) {
            if (g->adjMatrix[currVertex][i] == 1 && !visited[i]) {
                visited[i] = true;
                enqueue(&q, i);
            }
        }
    }

    return false;
}

bool bfsSearch_OMP(Graph *g, int startVertex, int targetVertex) {
    bool visited[g->vertexCount] = {false};
    memset(visited, false, g->vertexCount * sizeof(bool));

    Queue q;
    initQueue(&q, g->vertexCount);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    while (!isQueueEmpty(&q)) {
        int queueSize = queueLength(&q);
        int *layer = (int *)malloc(queueSize * sizeof(int));

        for (int i = 0; i < queueSize; i++) {
            layer[i] = dequeue(&q);
        }

        bool found = false;

        #pragma omp parallel for reduction(|:found)
        for (int i = 0; i < queueSize; i++) {
            int currVertex = layer[i];

            if (currVertex == targetVertex) {
                found |= true;
            }

            for (int j = 0; j < g->vertexCount; j++) {
                if (g->adjMatrix[currVertex][j] == 1 && !visited[j]) {
                    visited[j] = true;
                    enqueue(&q, j);
                }
            }
        }

        free(layer);

        if (found) {
            return true;
        }
    }

    return false;
}


