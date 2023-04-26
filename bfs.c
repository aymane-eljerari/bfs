#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#include "include/bfs.h"
#include "include/graph.h"
#include "include/queue.h"


/* Sequential BFS */
bool bfsSearch(Graph *g, int startVertex, int targetVertex) {
    bool visited[g->vertexCount];
    int i = 0;
    for (; i < g->vertexCount; i++) {
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

        for (i = 0; i < g->vertexCount; i++) {
            if (g->adjMatrix[currVertex][i] == 1 && !visited[i]) {
                visited[i] = true;
                enqueue(&q, i);
            }
        }
    }

    return false;
}

bool bfsSearch_OMP(Graph *g, int startVertex, int targetVertex) {
    bool visited[g->vertexCount];
    int i = 0;
    int j = 0;
    for (; i < g->vertexCount; i++) {
        visited[i] = false;
    }


    Queue q;
    initQueue(&q);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    while (!isQueueEmpty(&q)) {
        int queueSize = queueLength(&q);
        int *layer = (int *)malloc(queueSize * sizeof(int));

        for (i = 0; i < queueSize; i++) {
            layer[i] = dequeue(&q);
        }

        bool found = false;

        #pragma omp parallel for reduction(|:found)
        for (i = 0; i < queueSize; i++) {
            int currVertex = layer[i];

            if (currVertex == targetVertex) {
                found |= true;
            }

            for (j = 0; j < g->vertexCount; j++) {
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


