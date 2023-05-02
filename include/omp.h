#ifndef OMP_H
#define OMP_H

#include "graph.h"
#include "queue.h"

bool bfs_OMP(Graph *g, int startVertex, int targetVertex, int numThreads);

#endif