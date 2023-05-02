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

bool bfsSearch_OMP(Graph *g, int startVertex, int targetVertex, int numThreads) {
    bool visited[g->vertexCount];
    int i, j;
    for (i = 0; i < g->vertexCount; i++) {
        visited[i] = false;
    }

    Queue q;
    initQueue(&q);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    bool found = false; // shared variable to indicate whether targetVertex has been found

    while (!isQueueEmpty(&q)) {
        int currVertex = dequeue(&q);

        #pragma omp parallel for num_threads(numThreads) shared(found)
        for (j = 0; j < g->vertexCount; j++) {
            if (g->adjMatrix[currVertex][j] == 1 && !visited[j]) {
                visited[j] = true;
                enqueue(&q, j);
                if (j == targetVertex) {
                    found = true;
                }
            }
        }

        if (found) {
            break; // exit the while loop early if targetVertex has been found
        }
    }

    return found;
}
