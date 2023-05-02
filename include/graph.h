#ifndef GRAPH_H
#define GRAPH_H

typedef struct {
    int vertexCount;
    int **adjMatrix;
} Graph;

void initGraph(Graph *g, int vertexCount);
void addEdge(Graph *g, int src, int dest);
void generate(Graph *g, int vertexCount, int maxDegree);
void printAdjacencyMatrix(Graph *g);
void generateKNeighbors(Graph *g, int vertexCount, int k);

#endif
