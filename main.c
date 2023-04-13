
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"
#include "queue.h"
#include "bfs.h"

/* Test the algorithm */
int main() {
    Graph g;
    int vertexCount = 10;
    int maxDegree = 3;

    generate(&g, vertexCount, maxDegree);
    printAdjacencyMatrix(&g);

    return 0;
}
