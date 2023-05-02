#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "include/graph.h"
#include "include/queue.h"
#include "include/bfs.h"
#include "include/omp.h"
#include "include/pthreads.h"

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

    // int vertexCount = 100000;
    // int maxDegree = 100;

    // int start_vertex = 0;
    // int end_vertex = 99396;

    int vertexCount = 200000;
    int kNeighbors = 50;

    int start_vertex = 0;
    int end_vertex = 199396;



    struct timespec start, end;
    double time_serial, time_omp2, time_omp4, time_omp8, time_pt2, time_pt4, time_pt8;

    bool foundSerial, foundOMP2, foundOMP4, foundOMP8, foundPT2, foundPT4, foundPT8;
    

    printf("\nGenerating Graph with %d vertices..\n", vertexCount);
    
    generateKNeighbors(&g1, vertexCount, maxDegree);
    

    printf("\nExecuting BFS Serial\n\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundSerial = bfs(&g1, start_vertex, end_vertex);
    clock_gettime(CLOCK_REALTIME, &end);
    time_serial = interval(start, end);

    printf("Executing BFS 2 Thread OMP\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundOMP2 = bfs_OMP(&g1, start_vertex, end_vertex, 2);
    clock_gettime(CLOCK_REALTIME, &end);
    time_omp2 = interval(start, end);

    printf("Executing BFS 4 Thread OMP\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundOMP4 = bfs_OMP(&g1, start_vertex, end_vertex, 4);
    clock_gettime(CLOCK_REALTIME, &end);
    time_omp4 = interval(start, end);
    
    printf("Executing BFS 8 Thread OMP\n\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundOMP8 = bfs_OMP(&g1, start_vertex, end_vertex, 8);
    clock_gettime(CLOCK_REALTIME, &end);
    time_omp8 = interval(start, end);
    
    printf("Executing BFS 2 PThreads\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundPT2 = bfs_PTH(&g1, start_vertex, end_vertex, 2);
    clock_gettime(CLOCK_REALTIME, &end);
    time_pt2 = interval(start, end);

    printf("Executing BFS 4 PThreads\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundPT4 = bfs_PTH(&g1, start_vertex, end_vertex, 4);
    clock_gettime(CLOCK_REALTIME, &end);
    time_pt4 = interval(start, end);

    printf("Executing BFS 8 PThreads\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundPT8 = bfs_PTH(&g1, start_vertex, end_vertex, 8);
    clock_gettime(CLOCK_REALTIME, &end);
    time_pt8 = interval(start, end);

    
    if (foundSerial){
        printf("\nSerial time       : %f seconds\n\n", time_serial);
    }
    if (foundOMP2){
        printf("OMP time 2 threads  : %f seconds\n", time_omp2);
        printf("Speedup             : %.2f\n\n", time_serial/time_omp2);
    }
    if (foundOMP4){
        printf("OMP time 4 threads  : %f seconds\n", time_omp4);
        printf("Speedup             : %.2f\n\n", time_serial/time_omp4);
    }
    if (foundOMP8){
        printf("OMP time 8 threads  : %f seconds\n", time_omp8);
        printf("Speedup             : %.2f\n\n", time_serial/time_omp8);
    }
    if (foundPT2){
        printf("PThread 2 time      : %f seconds\n", time_pt2);
        printf("Speedup             : %.2f\n\n", time_serial/time_pt2);
    }
    if (foundPT4){
        printf("PThread 4 time      : %f seconds\n", time_pt4);
        printf("Speedup             : %.2f\n\n", time_serial/time_pt4);
    }
    if (foundPT8){
        printf("PThread 8 time      : %f seconds\n", time_pt8);
        printf("Speedup             : %.2f\n\n", time_serial/time_pt8);
    }
    else {
        printf("\nCould not find vertex\n");
    }
    

    return 0;
    
}
