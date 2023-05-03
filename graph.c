#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "include/constants.h"
#include "include/graph.h"


/* Graph operations */
void initGraph(Graph *g, int vertexCount) {
    // Initializes graph
    int i;
    g->vertexCount = vertexCount;
    g->adjMatrix = (int**)malloc(vertexCount * sizeof(int*));
    for (i = 0; i < vertexCount; i++) {
        g->adjMatrix[i] = (int*)calloc(vertexCount, sizeof(int));
    }
}

void addEdge(Graph *g, int src, int dest) {
    // Adds edge between src and dest vertices
    g->adjMatrix[src][dest] = 1;
    g->adjMatrix[dest][src] = 1;
}

void generateRandom(Graph *g, int vertexCount, int degree) {
    // Generates Random Graph
    int i, j;
    initGraph(g, vertexCount);
    
    if (RANDOM_GENERATE){srand(time(NULL));}
    else{srand(SEED);}

    for (i = 0; i < vertexCount; i++) {
        for (j = 0; j < degree; j++) {
            int dest = rand() % vertexCount;
            if (i != dest && g->adjMatrix[i][dest] == 0) {
                addEdge(g, i, dest);
            }
        }
    }
}

void generateKNeighbors(Graph *g, int vertexCount, int k) {
    // Generates Graph st. each random edge added is within k neighbors
    int i, j;
    initGraph(g, vertexCount);

    if (RANDOM_GENERATE) {
        srand(time(NULL));
    } else {
        srand(SEED);
    }

    for (i = 0; i < vertexCount; i++) {
        int start = i - k < 0 ? 0 : i - k;
        int end = i + k >= vertexCount ? vertexCount - 1 : i + k;
        int degree = rand() % (end - start + 1);

        for (j = 0; j < degree; j++) {
            int dest = rand() % (end - start + 1) + start;
            if (i != dest && g->adjMatrix[i][dest] == 0) {
                addEdge(g, i, dest);
            }
        }
    }
}

void printAdjacencyMatrix(Graph *g) {
    // Print the adjacency matrix of the graph
    int i, j, k;

    printf("\nAdjacency Matrix:\n\n   ");  
    for (k = 0; k < g->vertexCount; k++){
        printf("%d ", k);
    }
    printf("   \n  ");
    for (k = 0; k < g->vertexCount; k++){
        printf("--");
    }
    printf("\n");

    for (i = 0; i < g->vertexCount; i++){
        for (j = 0; j < g->vertexCount; j++){
            if(j == 0){
                printf("%d| ", i);
            }
            printf("%d ", g->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

void freeGraph(Graph *g) {
    // Frees the memory taken up by the graph
    int i;
    for (i = 0; i < g->vertexCount; i++) {
        free(g->adjMatrix[i]);
    }
    free(g->adjMatrix);
}
