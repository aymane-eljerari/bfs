#ifndef PTH_H
#define PTH_H

#include "graph.h"
#include "queue.h"

void *bfsThread(void *args);
bool bfs_PTH(Graph *g, int startVertex, int targetVertex, int numThreads);

#endif