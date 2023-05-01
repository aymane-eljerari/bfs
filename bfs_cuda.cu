/*
  To run CUDA program:

     nvcc -arch sm_35 bfs_cuda.cu -o bfs_cuda

     Need to add compiler options: -arch sm_35 (this is cuda compiler dependent)

*/
#include <stdio.h>
#include <stdlib.h>
// #include "include/graph.h"
#include <time.h>


#define BLOCKS 16
#define THREADS 256
#define MAX_VERTEX_COUNT 1000



/* Graph Generation Functions */

#define MAX_QUEUE_SIZE 10000000
#define RANDOM_GENERATE 0 // Whether to generate randomly or wrt to SEED.
#define SEED 42

typedef struct {
    int vertexCount;
    int **adjMatrix;
} Graph;

/* Graph operations */
void initGraph(Graph *g, int vertexCount) {
    int i;
    g->vertexCount = vertexCount;
    g->adjMatrix = (int**)malloc(vertexCount * sizeof(int*));
    for (i = 0; i < vertexCount; i++) {
        g->adjMatrix[i] = (int*)calloc(vertexCount, sizeof(int));
    }
}

void addEdge(Graph *g, int src, int dest) {
    g->adjMatrix[src][dest] = 1;
    g->adjMatrix[dest][src] = 1;
}

void generate(Graph *g, int vertexCount, int maxDegree) {
    int i, j;
    initGraph(g, vertexCount);
    
    if (RANDOM_GENERATE){srand(time(NULL));}
    else{srand(SEED);}

    for (i = 0; i < vertexCount; i++) {
        int degree = rand() % maxDegree;
        for (j = 0; j < degree; j++) {
            int dest = rand() % vertexCount;
            if (i != dest && g->adjMatrix[i][dest] == 0) {
                addEdge(g, i, dest);
            }
        }
    }
}

void printAdjacencyMatrix(Graph *g) {
    int i, j, k;

    printf("\nAdjacency Matrix:\n\n   ");  
    for (k = 0; k < g->vertexCount; k++){
        printf("%d ", k);
    }
    printf("   \n  ");
    for (k = 0; k < g->vertexCount; k++){
        printf("--");
    }
    printf("\n");

    for (i = 0; i < g->vertexCount; i++){
        for (j = 0; j < g->vertexCount; j++){
            if(j == 0){
                printf("%d| ", i);
            }
            printf("%d ", g->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

/* Queue Functions */
typedef struct {
    int front, rear;
    int data[MAX_QUEUE_SIZE];
} Queue;

/* Queue operations */
void initQueue(Queue *q) {
    q->front = q->rear = -1;
}

bool isQueueEmpty(Queue *q) {
    return (q->front == -1 && q->rear == -1);
}

bool isQueueFull(Queue *q) {
    return (q->rear == MAX_QUEUE_SIZE - 1);
}

int queueLength(Queue *q) {
    return (q->rear - q->front + 1);
}

void enqueue(Queue *q, int val) {
    if (isQueueFull(q)) {
        printf("Queue overflow\n");
        return;
    } else if (isQueueEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear++;
    }
    q->data[q->rear] = val;
}

int dequeue(Queue *q) {
    int val;
    if (isQueueEmpty(q)) {
        printf("Queue underflow\n");
        return -1;
    } else if (q->front == q->rear) {
        val = q->data[q->front];
        q->front = q->rear = -1;
    } else {
        val = q->data[q->front];
        q->front++;
    }
    return val;
}

void printQueue(Queue *q) {
    int i;
    printf("Queue: ");
    for (i = q->front; i <= q->rear; i++) {
        printf("%d ", q->data[i]);
    }
    printf("\n");
}

/********************************/


__device__ bool bfs(Graph *graph, int start, int finish) {
    int vertexCount = graph->vertexCount;
    int **adjMatrix = graph->adjMatrix;

    // Initialize visited and queue arrays
    __shared__ bool visited[MAX_VERTEX_COUNT];
    __shared__ int queue[MAX_VERTEX_COUNT];
    __shared__ int front;
    __shared__ int rear;

    for (int i = threadIdx.x; i < vertexCount; i += blockDim.x) {
        visited[i] = false;
    }

    if (threadIdx.x == 0) {
        visited[start] = true;
        front = -1;
        rear = -1;
        queue[++rear] = start;
    }

    __syncthreads();

    // Perform BFS
    while (true) {
        __syncthreads();

        if (front == rear) {
            // Queue is empty
            break;
        }

        int vertex = -1;

        if (threadIdx.x == 0) {
            // Dequeue a vertex from queue
            vertex = queue[++front];
        }
        printf("vertex: %d\n", vertex);

        __syncthreads();

        if (vertex == finish) {
            // Path found
            return true;
        }
        printf("threadidx: %d\n", threadIdx.x);
        printf("vertexCount: %d\n", vertexCount);
        for (int i = threadIdx.x; i < vertexCount; i += blockDim.x) {
            printf("adjMatrix[vertex][i]: %d\n", adjMatrix[vertex][i]);

            if (adjMatrix[vertex][i] == 1 && !visited[i]) {
                
                visited[i] = true;

                // Enqueue the vertex
                int newRear = atomicAdd(&rear, 1);
                queue[newRear] = i;
            }
        }
    }

    // Path not found
    return false;
}

__global__ void bfs_kernel(Graph *graph, int start, int finish, bool *found) {
    *found = bfs(graph, start, finish);
}



int main() {

    // Select GPU
    cudaSetDevice(0);

    // GPU Timing variables
    cudaEvent_t start, stop;
    float elapsed_gpu;

    // Allocate Host Memory
    Graph h_graph;
    int vertexCount = MAX_VERTEX_COUNT;
    int maxDegree = 10;

    // Initialize Host Memory
    generate(&h_graph, vertexCount, maxDegree);

    // printAdjacencyMatrix(&h_graph);

    // Allocate device memory for the Graph Struct
    Graph *d_graph;
    cudaMalloc(&d_graph, sizeof(Graph));

    // Allocate device memory for the array of pointers
    int **d_adjMatrix;
    cudaMalloc(&d_adjMatrix, sizeof(int*) * vertexCount);

    // Allocate device memory for the 2D array
    int *d_adjMatrix_data;
    cudaMalloc(&d_adjMatrix_data, sizeof(int) * vertexCount * vertexCount);

    // Copy the data to the device memory
    cudaMemcpy(d_adjMatrix_data, h_graph.adjMatrix, sizeof(int) * vertexCount * vertexCount, cudaMemcpyHostToDevice);
    for (int i = 0; i < vertexCount; i++) {
        cudaMemcpy(&d_adjMatrix[i], &d_adjMatrix_data[i * vertexCount], sizeof(int*), cudaMemcpyDeviceToHost);
    }

    // Initialize the device graph's adjMatrix pointer with the d_adjMatrix pointer
    cudaMemcpy(&d_graph->adjMatrix, &d_adjMatrix, sizeof(int **), cudaMemcpyDeviceToHost);

    // Copy host graph to the device graph
    cudaMemcpy(d_graph, &h_graph, sizeof(Graph), cudaMemcpyHostToDevice);

    // Free device memory for the adjacency matrix
    cudaFree(d_adjMatrix_data);


    bool h_found;
    bool *d_found;
    cudaMalloc(&d_found, sizeof(bool));


    /* GPU Implementation */

    // Create the CUDA events
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    int source = 5;
    int target = 8;

    /* Kernel */
    bfs_kernel<<<1, 16>>>(d_graph, source, target, d_found);
    // cudaDeviceSynchronize();

    cudaMemcpy(&h_found, d_found, sizeof(bool), cudaMemcpyDeviceToHost);

    if (h_found) {
        printf("Path found from %d to %d\n", source, target);
    } else {
        printf("No path found from %d to %d\n", source, target);
    }
    

    // Stop and destroy the timer
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsed_gpu, start, stop);
    printf("\nGPU time: %f (msec)\n", elapsed_gpu);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);


    // Free the memory
    cudaFree(d_graph);
    cudaFree(d_found);

    return 0;
}