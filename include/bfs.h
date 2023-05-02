#ifndef BFS_H
#define BFS_H

#include "graph.h"
#include "queue.h"

bool bfsSearch(Graph *g, int startVertex, int endVertex);
bool bfsSearch_OMP(Graph *g, int startVertex, int targetVertex, int numThreads);

#endif



