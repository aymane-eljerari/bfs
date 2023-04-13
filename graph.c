#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "graph.h"

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

void generate(Graph *g, int vertexCount, int maxDegree) {
    initGraph(g, vertexCount);
    srand(time(NULL));

    for (int i = 0; i < vertexCount; i++) {
        int degree = rand() % (maxDegree + 1);
        for (int j = 0; j < degree; j++) {
            int dest = rand() % vertexCount;
            if (i != dest && g->adjMatrix[i][dest] == 0) {
                addEdge(g, i, dest);
            }
        }
    }
}

void printAdjacencyMatrix(Graph *g) {
    for (int i = 0; i < g->vertexCount; i++){
        for (int j = 0; j < g->vertexCount; j++){
            printf("%d ", g->adjMatrix[i][j]);
        }
        printf("\n");
    }
}
