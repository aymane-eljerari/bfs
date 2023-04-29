/*
  To run CUDA program:

     nvcc -arch sm_35 bfs_cuda.cu -o bfs_cuda

     Need to add compiler options: -arch sm_35 (this is cuda compiler dependent)

*/
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include <time.h>


#define BLOCKS 16
#define THREADS 256


__global__ void kernel_cuda_simple(int **v_adj_begin, int **v_adj_list, int **result, int num_vertices, int n)
{
    /* 
        CUDA implementation 
        v_adj_list: concatenation of all adjacency lists of all vertices
        v_adj_begin: array of size V, storing offsets into previous array
        v_adj_length: array of size V, storing length of every adjacency list 
    */
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int num_threads = blockDim.x * gridDim.x

    for (int v = 0; v < num_vertices; v += num_threads)
    {
        int vertex = v + tid;

        if (vertex < num_vertices)
        {
            for (int n = 0; n < v_adj_length[vertex]; n++)
            {
                int neighbor = v_adj_list[v_adj_begin[vertex] + n]

                if (result[neighbor] > result[vertex] + 1)
                {
                    result[neighbor] = result[vertex] + 1;
                    *still_running = true;
                }
                }
        }
    }
}

void run()
{
    bool *still_running = true;

    while (*still_running)
    {
        cudaMemcpy(k_still_running, &false_value, sizeof(bool) * 1, cudaMemcpyHostToDevice);
        kernel_cuda_simple<<<BLOCKS, THREADS>>>();
        cudaMemcpy(still_running, k_still_running, sizeof(bool) * 1, cudaMemcpyDeviceToHost);
    }
    cudaThreadSynchronize();
}


__global__ void bfs_kernel(int *adj_matrix, int *queue, bool *visited, int *distance, int *parent, int source, int level, int num_vertices)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < level)
    {
        int node = queue[tid];
        visited[node] = true;

        for (int neighbor = 0; neighbor < num_vertices; neighbor++)
        {
            if (adj_matrix[node*num_vertices+neighbor] != 0 && !visited[neighbor])
            {
                visited[neighbor] = true;
                distance[neighbor] = distance[node] + 1;
                parent[neighbor] = node;
                queue[level++] = neighbor;
            }
        }
    }
}

__global__ void bfs(int *adj_matrix, int *queue, bool *visited, int *distance, int *parent, int source, int num_vertices)
{
    int level = 0;
    queue[level] = source;
    visited[source] = true;

    while (level < num_vertices)
    {
        int num_blocks = (level + BLOCK_SIZE - 1) / BLOCK_SIZE;
        bfs_kernel<<<num_blocks, BLOCK_SIZE>>>(adj_matrix, queue, visited, distance, parent, source, level, num_vertices);
        cudaDeviceSynchronize();
        level += BLOCK_SIZE * num_blocks;
    }
}



int main() {

    // Select GPU
    cudaSetDevice(0);

    // GPU Timing variables
    cudaEvent_t start, stop;
    float elapsed_gpu;

    // Allocate Host Memory
    Graph g;
    int vertexCount = 10000;
    int maxDegree = 5;

    // Initialize Host Memory
    generate(&g, vertexCount, maxDegree);

    size_t allocSize = vertexCount * sizeof(int);

    // Allocate Device Memory
    int *adjMatrix,

    cudaMalloc((void **)adjMatrix, allocSize);



    /* GPU Implementation: Global Memory */

    // Create the CUDA events
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    // Record event on the default stream
    cudaEventRecord(start, 0);

    // Initialize Device Memory
    cudaMemcpy(adjMatrix, g, allocSize, cudaMemcpyHostToDevice);

    dim3 dimGrid(ciel((float)vertexCount/(float)BLOCK_SIZE),ceil((float)vertexCount/(float)BLOCK_SIZE, 1));
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE, 1);
    

    /* Kernels */
    // bfs(int *adjacency_list, int *queue, bool *visited, int *distance, int *parent, int source, int num_vertices)
    bfs<<<dimGrid, dimBlock>>>()

    cudaMemcpy()
    // Stop and destroy the timer


    // Free the memory
    


    return 0;
}