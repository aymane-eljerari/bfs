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
#include "include/constants.h"

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

  double percentageList[] = {P1, P2, P3};

  int minVertexCount = C;
  int maxVertexCount = (A * (NUM_TESTS-1) *  (NUM_TESTS-1)) + (B * (NUM_TESTS-1)) + C;
  
  printf("\nPerforming Computations on %d graph sizes", NUM_TESTS);
  printf("\nVertex Number Count Range     : %d -> %d\n\n", minVertexCount, maxVertexCount);

  /*

  The data collection will be done in the following order, 
  for each combination of (algorithm, graph) pair, record 
  the execution times of different graph sizes.

  We ensure that all graphs are consistent, by using a SEED
  parameter to ensure that all functions operate on the exact
  same graphs.

              |    Serial    |      OMP*    |    PThreads*  |
  Random      |       0      |       1      |       2       |
  K Neighbors |       3      |       4      |       5       |

  * Parallel Algorithms are tested with NUM_THREADS above

  */


  Graph g;
  struct timespec start, end;
  bool found;

  int c, n;
  
  for (c = 6; c >= 0; c--) {
    if (c == 5){
      printf("Executing %d PThreads - K Neighbors\n\n", NUM_THREADS);
      int x, p;
      double time_stamp[NUM_TESTS][P_LENGTH];
      // Loop over graph and parameter sizes
      for (x = 0; x < NUM_TESTS && (n = A*x*x + B*x + C); x++){
        printf("ITERATIONS %d\n", x);
        for (p = 0; p < P_LENGTH; p++){
          generateKNeighbors(&g, n, (int)(percentageList[p] * n));
          
          // Pick start and End Vertices
          int vS = 0;
          int vE = (int)n - (n*0.01);

          clock_gettime(CLOCK_REALTIME, &start);
          found = bfs_PTH(&g, vS, vE, NUM_THREADS);
          clock_gettime(CLOCK_REALTIME, &end);
          time_stamp[x][p] = interval(start, end);

          if (!found) printf("No path found between %d and %d\n", vS, vE);
          freeGraph(&g);
        }
      }

      // Print Results
      int a,b,c;
      printf("\nrow_length, ");
      for (a = 0 ; a < P_LENGTH; a++){
        if (a != 0) printf(", ");
        printf("%d%%", (int)(percentageList[a]*100));
      }
      printf("\n");

      for (b = 0; b < NUM_TESTS && (n = A*b*b + B*b + C); b++){
        printf("%d, ", n);
        for (c = 0 ; c < P_LENGTH; c++){
          if (c != 0) printf(", ");
          printf("%.8f", time_stamp[b][c]);
        }
        printf("\n");
      }
      printf("\n\n");

    }
    else if (c == 4){
      printf("Executing %d OMP - K Neighbors\n\n", NUM_THREADS);
      int x, p;
      double time_stamp[NUM_TESTS][P_LENGTH];
      // Loop over graph and parameter sizes
      for (x = 0; x < NUM_TESTS && (n = A*x*x + B*x + C); x++){
        printf("ITERATIONS %d\n", x);
        for (p = 0; p < P_LENGTH; p++){
          generateKNeighbors(&g, n, (int)(percentageList[p] * n));
          // Pick start and End Vertices
          int vS = 0;
          int vE = n - (n*0.05);

          clock_gettime(CLOCK_REALTIME, &start);
          found = bfs_OMP(&g, vS, vE, NUM_THREADS);
          clock_gettime(CLOCK_REALTIME, &end);
          time_stamp[x][p] = interval(start, end);
          if (!found) printf("No path found between %d and %d\n", vS, vE);
          freeGraph(&g);
        }
      }

      // Print Results
      int a,b,c;
      printf("\nrow_length, ");
      for (a = 0 ; a < P_LENGTH; a++){
        if (a != 0) printf(", ");
        printf("%d%%", (int)(percentageList[a]*100));
      }
      printf("\n");

      for (b = 0; b < NUM_TESTS && (n = A*b*b + B*b + C); b++){
        printf("%d, ", n);
        for (c = 0 ; c < P_LENGTH; c++){
          if (c != 0) printf(", ");
          printf("%.8f", time_stamp[b][c]);
        }
        printf("\n");
      }
      printf("\n\n");
   
    }
    else if (c == 3){
      printf("Executing Serial - K Neighbors\n\n");
      int x, p;
      double time_stamp[NUM_TESTS][P_LENGTH];
      // Loop over graph and parameter sizes
      for (x = 0; x < NUM_TESTS && (n = A*x*x + B*x + C); x++){
        printf("ITERATIONS %d\n", x);
        for (p = 0; p < P_LENGTH; p++){
          generateKNeighbors(&g, n, (int)(percentageList[p] * n));
          // Pick start and End Vertices
          int vS = 0;
          int vE = n - (n*0.05);

          clock_gettime(CLOCK_REALTIME, &start);
          found = bfs(&g, vS, vE);
          clock_gettime(CLOCK_REALTIME, &end);
          time_stamp[x][p] = interval(start, end);

          if (!found) printf("No path found between %d and %d\n", vS, vE);
          freeGraph(&g);
        }
      }

      // Print Results
      int a,b,c;
      printf("\nrow_length, ");
      for (a = 0 ; a < P_LENGTH; a++){
        if (a != 0) printf(", ");
        printf("%d%%", (int)(percentageList[a]*100));
      }
      printf("\n");

      for (b = 0; b < NUM_TESTS && (n = A*b*b + B*b + C); b++){
        printf("%d, ", n);
        for (c = 0 ; c < P_LENGTH; c++){
          if (c != 0) printf(", ");
          printf("%.8f", time_stamp[b][c]);
        }
        printf("\n");
      }
      printf("\n\n");

      
    }
    else if (c == 2){
      printf("Executing %d PThreads - Random\n\n", NUM_THREADS);
      int x, p;
      double time_stamp[NUM_TESTS][P_LENGTH];
      // Loop over graph and parameter sizes
      for (x = 0; x < NUM_TESTS && (n = A*x*x + B*x + C); x++){
        printf("ITERATIONS %d\n", x);
        for (p = 0; p < P_LENGTH; p++){
          generateRandom(&g, n, (int)(percentageList[p] * n));
          // Pick start and End Vertices
          int vS = 0;
          int vE = n - (n*0.05);

          clock_gettime(CLOCK_REALTIME, &start);
          found = bfs_PTH(&g, vS, vE, NUM_THREADS);
          clock_gettime(CLOCK_REALTIME, &end);
          time_stamp[x][p] = interval(start, end);

          if (!found) printf("No path found between %d and %d\n", vS, vE);
          freeGraph(&g);
        }
      }

      // Print Results
      int a,b,c;
      printf("\nrow_length, ");
      for (a = 0 ; a < P_LENGTH; a++){
        if (a != 0) printf(", ");
        printf("%d%%", (int)(percentageList[a]*100));
      }
      printf("\n");

      for (b = 0; b < NUM_TESTS && (n = A*b*b + B*b + C); b++){
        printf("%d, ", n);
        for (c = 0 ; c < P_LENGTH; c++){
          if (c != 0) printf(", ");
          printf("%.8f", time_stamp[b][c]);
        }
        printf("\n");
      }
      printf("\n\n");
      
    }
    else if (c == 1){
      printf("Executing %d OMP - Random\n\n", NUM_THREADS);
      int x, p;
      double time_stamp[NUM_TESTS][P_LENGTH];
      // Loop over graph and parameter sizes
      for (x = 0; x < NUM_TESTS && (n = A*x*x + B*x + C); x++){
        printf("ITERATIONS %d\n", x);
        for (p = 0; p < P_LENGTH; p++){
          generateRandom(&g, n, (int)(percentageList[p] * n));
          // Pick start and End Vertices
          int vS = 0;
          int vE = n - (n*0.05);

          clock_gettime(CLOCK_REALTIME, &start);
          found = bfs_OMP(&g, vS, vE, NUM_THREADS);
          clock_gettime(CLOCK_REALTIME, &end);
          time_stamp[x][p] = interval(start, end);

          if (!found) printf("No path found between %d and %d\n", vS, vE);
          freeGraph(&g);
        }
      }

      // Print Results
      int a,b,c;
      printf("\nrow_length, ");
      for (a = 0 ; a < P_LENGTH; a++){
        if (a != 0) printf(", ");
        printf("%d%%", (int)(percentageList[a]*100));
      }
      printf("\n");

      for (b = 0; b < NUM_TESTS && (n = A*b*b + B*b + C); b++){
        printf("%d, ", n);
        for (c = 0 ; c < P_LENGTH; c++){
          if (c != 0) printf(", ");
          printf("%.8f", time_stamp[b][c]);
        }
        printf("\n");
      }
      printf("\n\n");
      
    }
    else if (c == 0){
      printf("Executing Serial - Random\n\n");
      int x, p;
      double time_stamp[NUM_TESTS][P_LENGTH];
      // Loop over graph and parameter sizes
      for (x = 0; x < NUM_TESTS && (n = A*x*x + B*x + C); x++){
        printf("ITERATIONS %d\n", x);
        for (p = 0; p < P_LENGTH; p++){
          generateRandom(&g, n, (int)(percentageList[p] * n));
          // Pick start and End Vertices
          int vS = 0;
          int vE = n - (n*0.05);

          clock_gettime(CLOCK_REALTIME, &start);
          found = bfs(&g, vS, vE);
          clock_gettime(CLOCK_REALTIME, &end);
          time_stamp[x][p] = interval(start, end);

          if (!found) printf("No path found between %d and %d\n", vS, vE);
          freeGraph(&g);
        }
      }

      // Print Results
      int a,b,c;
      printf("\nrow_length, ");
      for (a = 0 ; a < P_LENGTH; a++){
        if (a != 0) printf(", ");
        printf("%d%%", (int)(percentageList[a]*100));
      }
      printf("\n");

      for (b = 0; b < NUM_TESTS && (n = A*b*b + B*b + C); b++){
        printf("%d, ", n);
        for (c = 0 ; c < P_LENGTH; c++){
          if (c != 0) printf(", ");
          printf("%.8f", time_stamp[b][c]);
        }
        printf("\n");
      }
      printf("\n\n");
      
    }
  }

    return 0;
    
}
