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

// /* Parallel BFS */
// bool bfsSearch_OMP(Graph *g, int startVertex, int targetVertex) {
//     bool visited[g->vertexCount];
//     int i, j, k, l, queueSize, *layer;

//     for (i = 0; i < g->vertexCount; i++) {
//         visited[i] = false;
//     }

//     Queue q;
//     initQueue(&q);

//     visited[startVertex] = true;
//     enqueue(&q, startVertex);


//     while (!isQueueEmpty(&q)) {
//         printQueue(&q);
//         queueSize = queueLength(&q);
//         layer = (int *)malloc(queueSize * sizeof(int));
        
//         printf("Layer Vertices: ");

//         for (j = 0; j < queueSize; j++) {
//             layer[j] = dequeue(&q);
//             printf("%d ", layer[j]);
//         }
//         printf("\n");


//         bool found = false;
//         #pragma omp parallel for reduction(|:found)
//         for (k = 0; k < queueSize; k++) {
//             int currVertex = layer[k];
//             printf("Current Vertex: %d\n", currVertex);

//             if (currVertex == targetVertex) {
//                 found |= true;
//             }

//             for (l = 0; l < g->vertexCount; l++) {
//                 if (g->adjMatrix[currVertex][l] == 1 && !visited[l]) {
//                     visited[l] = true;
//                     printf("Added Vertex %d\n", l);
//                     enqueue(&q, l);
                    
//                 }
//             }
//         }
//         free(layer);

//         if (found) {
//             return true;
//         }
//     }

//     return false;
// }

bool bfsSearch_OMP(Graph *g, int startVertex, int targetVertex) {
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

        #pragma omp parallel for shared(found)
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
