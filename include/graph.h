#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int vertexCount;
    int **adjMatrix;
} Graph;

void initGraph(Graph *g, int vertexCount);
void addEdge(Graph *g, int src, int dest);
void generateRandom(Graph *g, int vertexCount, int degree);
void printAdjacencyMatrix(Graph *g);
void generateKNeighbors(Graph *g, int vertexCount, int k);
void freeGraph(Graph *g);

#endif
