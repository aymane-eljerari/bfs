#include "include/graph.h"
#include "include/queue.h"
#include "include/bfs.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define _POSIX_C_SOURCE 200809L


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
    double timeElapsed_serial, timeElapsed_OMP;

    bool foundSerial, foundOMP;

    generate(&g, vertexCount, maxDegree);

    clock_gettime(CLOCK_REALTIME, &start);
    foundSerial = bfsSearch(&g, 0, 230);
    clock_gettime(CLOCK_REALTIME, &end);


    // clock_gettime(CLOCK_REALTIME, &start);
    // foundOMP = bfsSearch_OMP(&g, 0, 230);
    // clock_gettime(CLOCK_REALTIME, &end);

    timeElapsed_serial = interval(start, end) / 1000;
    timeElapsed_OMP = interval(start, end) / 1000;


    if (foundSerial){
        printf("\nSerial time elapsed: %f ms\n", timeElapsed_serial);
        printf("\nSerial time elapsed: %f ms\n", timeElapsed_OMP);
    }
    else{
        printf("\nCould not find vertex\n");
    }
    

    return 0;
}
