#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "include/graph.h"
#include "include/queue.h"
#include "include/bfs.h"

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
    Graph g1, g2;

    int vertexCount = 80000;
    int maxDegree = 50;

    int start_vertex = 0;
    int end_vertex = 34018;

    struct timespec start_S, end_S, start_P, end_P;
    double timeElapsed_serial, timeElapsed_parallel;

    bool foundSerial, foundParallel;
    

    printf("\nGenerating Graph with %d vertices..\n", vertexCount);
    generate(&g1, vertexCount, maxDegree);
    generate(&g2, vertexCount, maxDegree);
    
    // printAdjacencyMatrix(&g1);
    // printAdjacencyMatrix(&g2);

    printf("\nRunning Serial BFS\n");
    clock_gettime(CLOCK_REALTIME, &start_S);
    foundSerial = bfsSearch(&g1, start_vertex, end_vertex);
    clock_gettime(CLOCK_REALTIME, &end_S);

    printf("\nRunning Parallel BFS\n");
    clock_gettime(CLOCK_REALTIME, &start_P);
    foundParallel = bfsSearch_OMP(&g2, start_vertex, end_vertex);
    clock_gettime(CLOCK_REALTIME, &end_P);
    

    if (foundSerial){
        timeElapsed_serial = interval(start_S, end_S);
        printf("\nSerial time   : %f seconds\n", timeElapsed_serial);

    }
    if (foundParallel){
        timeElapsed_parallel = interval(start_P, end_P);
        printf("\nParallel time : %f seconds\n", timeElapsed_parallel);
        printf("\nSpeedup : %.2f\n\n", timeElapsed_serial/timeElapsed_parallel);
    }
    else{
        printf("\nCould not find vertex\n");
    }
    

    return 0;
    
}
