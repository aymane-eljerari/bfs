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

// __global__ void kernel_cuda_simple(int **v_adj_begin, int **v_adj_list, int **result, int num_vertices, int n)
// {
//     /* 
//         CUDA implementation 
//         v_adj_list: concatenation of all adjacency lists of all vertices
//         v_adj_begin: array of size V, storing offsets into previous array
//         v_adj_length: array of size V, storing length of every adjacency list 
//     */
//     int tid = blockIdx.x * blockDim.x + threadIdx.x;
//     int num_threads = blockDim.x * gridDim.x

//     for (int v = 0; v < num_vertices; v += num_threads)
//     {
//         int vertex = v + tid;

//         if (vertex < num_vertices)
//         {
//             for (int n = 0; n < v_adj_length[vertex]; n++)
//             {
//                 int neighbor = v_adj_list[v_adj_begin[vertex] + n]

//                 if (result[neighbor] > result[vertex] + 1)
//                 {
//                     result[neighbor] = result[vertex] + 1;
//                     *still_running = true;
//                 }
//                 }
//         }
//     }
// }

// void run()
// {
//     bool *still_running = true;

//     while (*still_running)
//     {
//         cudaMemcpy(k_still_running, &false_value, sizeof(bool) * 1, cudaMemcpyHostToDevice);
//         kernel_cuda_simple<<<BLOCKS, THREADS>>>();
//         cudaMemcpy(still_running, k_still_running, sizeof(bool) * 1, cudaMemcpyDeviceToHost);
//     }
//     cudaThreadSynchronize();
// }


// __global__ void bfs_kernel(int *adj_matrix, int *queue, bool *visited, int *distance, int *parent, int source, int level, int num_vertices)
// {
//     int tid = blockIdx.x * blockDim.x + threadIdx.x;

//     if (tid < level)
//     {
//         int node = queue[tid];
//         visited[node] = true;

//         for (int neighbor = 0; neighbor < num_vertices; neighbor++)
//         {
//             if (adj_matrix[node*num_vertices+neighbor] != 0 && !visited[neighbor])
//             {
//                 visited[neighbor] = true;
//                 distance[neighbor] = distance[node] + 1;
//                 parent[neighbor] = node;
//                 queue[level++] = neighbor;
//             }
//         }
//     }
// }

// __global__ void bfs(int *adj_matrix, int *queue, bool *visited, int *distance, int *parent, int source, int num_vertices)
// {
//     int level = 0;
//     queue[level] = source;
//     visited[source] = true;

//     while (level < num_vertices)
//     {
//         int num_blocks = (level + BLOCK_SIZE - 1) / BLOCK_SIZE;
//         bfs_kernel<<<num_blocks, BLOCK_SIZE>>>(adj_matrix, queue, visited, distance, parent, source, level, num_vertices);
//         cudaDeviceSynchronize();
//         level += BLOCK_SIZE * num_blocks;
//     }
// }


// void bfs_omp(Graph *g, int startVertex, int targetVertex)
// {

//     bool visited[g->vertexCount];
//     int i, j;
//     for (i = 0; i < g->vertexCount; i++) {
//         visited[i] = false;
//     }

//     Queue q;
//     initQueue(&q);

//     visited[startVertex] = true;
//     enqueue(&q, startVertex);

//     bool found = false; // shared variable to indicate whether targetVertex has been found

//     while (!isQueueEmpty(&q)) {
//         int currVertex = dequeue(&q);

//         #pragma omp parallel for shared(found)
//         for (j = 0; j < g->vertexCount; j++) {
//             if (g->adjMatrix[currVertex][j] == 1 && !visited[j]) {
//                 visited[j] = true;
//                 enqueue(&q, j);
//                 if (j == targetVertex) {
//                     found = true;
//                 }
//             }
//         }

//         if (found) {
//             break; // exit the while loop early if targetVertex has been found
//         }
//     }

//     return found;
// }

__global__ void bfs_kernel(Graph *d_graph)
{
    int vertexCount = d_graph->vertexCount;
    int **adjMatrix= d_graph->adjMatrix;

    printf("vertexCount: %d\n", vertexCount);

}



int main() {

    // Select GPU
    cudaSetDevice(0);

    // GPU Timing variables
    // cudaEvent_t start, stop;
    // float elapsed_gpu;

    // Allocate Host Memory
    Graph h_graph;
    int vertexCount = 10;
    int maxDegree = 5;

    // Initialize Host Memory
    generate(&h_graph, vertexCount, maxDegree);

    printAdjacencyMatrix(&h_graph);

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



    /* GPU Implementation */

    // Create the CUDA events
    // cudaEventCreate(&start);
    // cudaEventCreate(&stop);

    // cudaEventRecord(start, 0);

    // Initialize Device Memory


    /* Kernel */
    bfs_kernel<<<1,1>>>(d_graph);
    

    // Stop and destroy the timer
    // cudaEventRecord(stop,0);
    // cudaEventSynchronize(stop);
    // cudaEventElapsedTime(&elapsed_gpu, start, stop);
    // printf("\nGPU time: %f (msec)\n", elapsed_gpu);
    // cudaEventDestroy(start);
    // cudaEventDestroy(stop);


    // Free the memory
    cudaFree(d_graph);


    return 0;
}