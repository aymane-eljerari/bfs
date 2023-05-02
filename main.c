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
    Graph g1;

    int vertexCount = 60000;
    int maxDegree = 20;

    int start_vertex = 0;
    int end_vertex = 49392;

    struct timespec start, end;
    double time_serial, time_parallel2, time_parallel4, time_parallel8;

    bool foundSerial, foundParallel2, foundParallel4, foundParallel8;
    

    printf("\nGenerating Graph with %d vertices..\n", vertexCount);
    generate(&g1, vertexCount, maxDegree);
    
    // printAdjacencyMatrix(&g1);

    printf("\nExecuting Serial BFS\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundSerial = bfsSearch(&g1, start_vertex, end_vertex);
    clock_gettime(CLOCK_REALTIME, &end);
    time_serial = interval(start, end);

    printf("Executing 2 Thread Parallel BFS\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundParallel2 = bfsSearch_OMP(&g1, start_vertex, end_vertex, 2);
    clock_gettime(CLOCK_REALTIME, &end);
    time_parallel2 = interval(start, end);

    printf("Executing 4 Thread Parallel BFS\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundParallel4 = bfsSearch_OMP(&g1, start_vertex, end_vertex, 4);
    clock_gettime(CLOCK_REALTIME, &end);
    time_parallel4 = interval(start, end);
    
    printf("Executing 8 Thread Parallel BFS\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundParallel8 = bfsSearch_OMP(&g1, start_vertex, end_vertex, 8);
    clock_gettime(CLOCK_REALTIME, &end);
    time_parallel8 = interval(start, end);
    
    
    if (foundSerial){
        printf("\nSerial time   : %f seconds\n", time_serial);

    }
    if (foundParallel2){
        printf("Parallel time 2 threads: %f seconds\n", time_parallel2);
        printf("\nSpeedup : %.2f\n\n", time_serial/time_parallel2);
    }
    if (foundParallel4){
        printf("Parallel time 4 threads: %f seconds\n", time_parallel4);
        printf("\nSpeedup : %.2f\n\n", time_serial/time_parallel4);
    }
    if (foundParallel8){
        printf("Parallel time 8 threads: %f seconds\n", time_parallel8);
        printf("\nSpeedup : %.2f\n\n", time_serial/time_parallel8);
    }
    else{
        printf("\nCould not find vertex\n");
    }
    

    return 0;
    
}
