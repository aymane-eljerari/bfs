
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#include "graph.h"
#include "queue.h"
#include "bfs.h"

double interval(struct timespec start, struct timespec end){
  struct timespec temp;
  temp.tv_sec = end.tv_sec - start.tv_sec;
  temp.tv_nsec = end.tv_nsec - start.tv_nsec;
  if (temp.tv_nsec < 0) {
    temp.tv_sec = temp.tv_sec - 1;
    temp.tv_nsec = temp.tv_nsec + 1000000000;
  }
  return (((double)temp.tv_sec) + ((double)temp.tv_nsec)*1.0e-9);
}

/* Test the algorithm */
int main() {

    Graph g;
    int vertexCount = 10000;
    int maxDegree = 5;

    struct timespec start, end;
    double timeElapsed;

    bool found;

    generate(&g, vertexCount, maxDegree);

    clock_gettime(CLOCK_MONOTONIC, &start);
    found = bfsSearch(&g, 0, 230);
    clock_gettime(CLOCK_MONOTONIC, &end);

    timeElapsed = interval(start, end) / 1000;

    if (found){
        printf("\nTime elapsed: %f ms\n", timeElapsed);
    }
    else{
        printf("\nCould not find vertex\n");
    }
    

    return 0;
}
