
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"
#include "queue.h"
#include "bfs.h"

/* Test the algorithm */
int main() {
    Graph g;
    int vertexCount = 5;
    int maxDegree = 3;

    generate(&g, vertexCount, maxDegree);
    printAdjacencyMatrix(&g);

    // initGraph(&g, vertexCount);

    // addEdge(&g, 0, 1);
    // addEdge(&g, 0, 2);
    // addEdge(&g, 1, 3);
    // addEdge(&g, 1, 4);
    // addEdge(&g, 2, 4);
    // addEdge(&g, 3, 4);
    // addEdge(&g, 3, 5);

    // printf("BFS Traversal: ");
    // bfs(&g, 0);

    return 0;
}
