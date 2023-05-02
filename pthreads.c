#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#include "include/graph.h"
#include "include/queue.h"

#define NUM_THREADS 4 

typedef struct {
    Graph *g;
    Queue *q;
    bool *visited;
    int targetVertex;
    bool *found;
    pthread_mutex_t *mutex;
} ThreadArgs;

void *bfsThread(void *args) {
    ThreadArgs *targs = (ThreadArgs *) args;

    while (1) {
        pthread_mutex_lock(targs->mutex);

        if (isQueueEmpty(targs->q) || *(targs->found)) {
            // either the queue is empty or the targetVertex has been found
            pthread_mutex_unlock(targs->mutex);
            break;
        }

        int currVertex = dequeue(targs->q);

        pthread_mutex_unlock(targs->mutex);

        int i;
        for (i = 0; i < targs->g->vertexCount; i++) {
            if (targs->g->adjMatrix[currVertex][i] == 1 && !(targs->visited[i])) {
                pthread_mutex_lock(targs->mutex);
                targs->visited[i] = true;
                enqueue(targs->q, i);

                if (i == targs->targetVertex) {
                    *(targs->found) = true;
                }

                pthread_mutex_unlock(targs->mutex);
            }
        }
    }

    pthread_exit(NULL);
}

bool bfs_PTH(Graph *g, int startVertex, int targetVertex, int numThreads) {
    bool visited[g->vertexCount];
    int i;
    for (i = 0; i < g->vertexCount; i++) {
        visited[i] = false;
    }

    Queue q;
    initQueue(&q);

    visited[startVertex] = true;
    enqueue(&q, startVertex);

    bool found = false; // shared variable to indicate whether targetVertex has been found

    // create threads
    pthread_t threads[numThreads];
    ThreadArgs targs[numThreads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    for (i = 0; i < numThreads; i++) {
        targs[i].g = g;
        targs[i].q = &q;
        targs[i].visited = visited;
        targs[i].targetVertex = targetVertex;
        targs[i].found = &found;
        targs[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, bfsThread, (void *) &targs[i]);
    }

    // wait for threads to finish
    for (i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    return found;
}
