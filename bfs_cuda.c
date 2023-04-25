#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime>


__global__ void bfs_kernel(int *adjacency_list, int *distances, int *queue, int source_node, int num_nodes) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid == source_node) {
        distances[tid] = 0;
        queue[0] = tid;
    } else {
        distances[tid] = -1;
    }
    __syncthreads();
    int queue_start = 0;
    int queue_end = 1;
    while (queue_start < queue_end) {
        int current_node = queue[queue_start];
        queue_start++;
        int start_index = adjacency_list[current_node];
        int end_index = (current_node == num_nodes - 1) ? blockDim.x * blockIdx.x + threadIdx.x : adjacency_list[current_node + 1];
        for (int neighbor_index = start_index; neighbor_index < end_index; neighbor_index++) {
            int neighbor = adjacency_list[neighbor_index];
            if (distances[neighbor] == -1) {
                distances[neighbor] = distances[current_node] + 1;
                queue[queue_end] = neighbor;
                queue_end++;
            }
        }
        __syncthreads();
    }
}


int main() {
    int num_nodes = 6;
    int num_edges = 7;
    int source_node = 0;
    int max_degree = 2;

    // Set up the graph as an adjacency list
    int *adjacency_list = (int*) malloc(num_edges * sizeof(int));
    adjacency_list[0] = 1;
    adjacency_list[1] = 2;
    adjacency_list[2] = 3;
    adjacency_list[3] = 0;
    adjacency_list[4] = 2;
    adjacency_list[5] = 4;
    adjacency_list[6] = 5;

    // Set up the distances and queue arrays
    int *distances = (int*) malloc(num_nodes * sizeof(int));
    int *queue = (int*) malloc(num_nodes * sizeof(int));

    // Set up the CUDA device arrays
    int *d_adjacency_list, *d_distances, *d_queue;
    cudaMalloc(&d_adjacency_list, num_edges * sizeof(int));
    cudaMalloc(&d_distances, num_nodes * sizeof(int));
    cudaMalloc(&d_queue, num_nodes * sizeof(int));

    // Copy the graph to the CUDA device
    cudaMemcpy(d_adjacency_list, adjacency_list, num_edges * sizeof(int), cudaMemcpyHostToDevice);

    // Run the BFS kernel
    bfs_kernel<<<1, num_nodes>>>(d_adjacency_list, d_distances, d_queue, source_node, num_nodes);

    // Copy the results back from the CUDA device
    cudaMemcpy(distances, d_distances, num_nodes * sizeof(int), cudaMemcpyDeviceToHost);

    // Print the results
    printf("Distances from node %d:\n", source_node);
    for (int i = 0; i < num_nodes; i++) {
        printf("%d: %d\n", i, distances[i]);
    }

    // Free the memory
    free(adjacency_list);
    free(distances);
    free(queue);
    cudaFree(d_adjacency_list);
    cudaFree(d_distances);
    cudaFree(d_queue);

    return 0;
}