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
    Graph g;
    int vertexCount = 100;
    int maxDegree = 10;
    struct timespec start, end;
    double timeElapsed_serial;

    bool foundSerial;

    printf("\nGenerating graph...\n");
    generate(&g, vertexCount, maxDegree);
    // printAdjacencyMatrix(&g);

    printf("\nRunning Serial BFS\n");
    clock_gettime(CLOCK_REALTIME, &start);
    foundSerial = bfsSearch(&g, 0, 10);
    clock_gettime(CLOCK_REALTIME, &end);
    

    if (foundSerial){
        timeElapsed_serial = interval(start, end) / 1000000;
        printf("\nSerial time   : %f µs\n", timeElapsed_serial);
    }
    else{
        printf("\nCould not find vertex\n");
    }
    

    return 0;
    
}
