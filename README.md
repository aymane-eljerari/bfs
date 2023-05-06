


# 1. Description<a name="_page2_x72.00_y72.00"></a> of the Algorithm

Breadth-First Search is a graph traversing algorithm used as a common building block for more complex algorithm designs. Its time complexity is O(V + E), and its space complexity is O(V). Breadth-First Search joins a particular set of problems spanning sparse tree traversal problems due to its use in finding minimum spanning trees in more complex algorithm designs. Furthermore, the processing of nodes in the algorithm poses many challenges to parallelization. For example, sparse graphs make it difficult to fully utilize the processing power of a parallel implementation because an explored node may or may not have many neighboring nodes to process with multiple threads. This reason and others thwarted our algorithmic analysis of the GPU implementation. However, we created multithreaded versions, discussed further below.

## 1.1 Code<a name="_page2_x72.00_y237.33"></a> Implementation

There are multiple ways to organize graph structures with advantages and disadvantages depending on the algorithm, the density of the graph, and the memory used to store the graph data. In our implementations, we used a 2-dimensional array to represent the graph when working with the CPU. This structure worked nicely with the serial and multithreaded versions of the breadth-first search algorithm. However, we converted the 2-dimensional array into a 1-dimensional array for processing by the GPU.

## 1.2 Graph<a name="_page2_x72.00_y366.41"></a> Challenges

As mentioned in the description of the algorithm section, one challenge of parallelizing breadth-first search is representing the sparseness of the graph structure in memory. This problem makes it difficult to traverse node layers of the graph because of the growing complexity of handling thread conflicts and traversing multiple paths simultaneously. If we think about the graph pictorially, where we have a starting source node, we can visualize the traversal of nodes as an exploration of the layers in sections of the graph. This exploration of nodes differentiates breadth-first search from depth-first search and why we have to solve the challenge of controlling node traversal after exploring a certain distance. (load balance) Furthermore, we must devise a different way of representing the graph to utilize the multiple threads fully. Otherwise, we run into conflicts with traversing nodes twice, increasing time complexity, although the speed might be faster.

To address the issue of thread utilization, we can use a compressed sparse row sparse matrix format to store the array in memory as two arrays.[^1] Also, to control for the layers of nodes explored, we need to devise a solution for the load-balancing issue we run into with multiple threads exploring nodes. This issue is challenging because of the lack of control where some threads may have many neighbors to explore, whereas others might have few nodes. These threads would cause a barrier synchronization problem where finished threads wait long for the other threads to explore their layers.

In summary, if optimized for performance, these challenges make it easier to justify parallelizing the breadth-first search algorithm. Otherwise, the time complexity increases, and you do less meaningful work.

# 2. **Graph<a name="_page3_x72.00_y119.61"></a> Generation**
## 2.1 Graph<a name="_page3_x72.00_y158.12"></a> Types

To test for performance differences, we ensure that all graphs generated are identical for the different trials. We do so by generating numbers pseudo-randomly with the help of a SEED. Both functions take three inputs; an initialized graph, the number of vertices, and a degree or k neighbor value. We test our algorithms using a percentage value representing the relationship between the size of the adjacency matrix and the number of connections.

e.g. A matrix of size 100 by 100 containing 15 edges per vertex would have a 15% connection

## 2.2 Random

<a name="_page3_x72.00_y299.07"></a>This function uses the degree input to generate graphs such that each vertex has precise *degree* connections with other vertices.

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.001.png)

**Figure 1:** generateRandom() code snippet

## 2.3 K<a name="_page4_x72.00_y87.87"></a> Neighbors

This function only adds edges between vertices within k neighbors of the current vertex and generates diagonal-looking connections.

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.002.png)

**Figure 2:** generateKNeighbors() code snippet

# 3. CPU

##  <a name="_page4_x72.00_y516.59"></a><a name="_page4_x72.00_y555.11"></a>3.1 Serial

The function receives three inputs, graph *G*, start and target vertices. The graph traversal is performed within the while loop. The current vertex being processed is the most recent dequeued vertex. While the queue is not empty, we enqueue all nodes connected to the current vertex and mark them as visited to avoid processing them a second time.

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.003.png)

**Figure 3:** Serial algorithm code snippet

## <a name="_page5_x72.00_y541.17"></a>3.1.1 Results:

The serial version takes longer to execute graphs with higher percentages, as it needs to process more edges before reaching the target vertex. There is also a difference in execution time between K Neighbors and Random. Because of the more structured nature of the K Neighbors algorithm, the process benefits more from the spatial locality of the visited array. Although *generateKNeighbors()* runs faster overall, we notice some trials in which *generateRandom()* executes more quickly. This increase in speed is because, for some tests, the random graph generation leads to a shorter path between the start and target vertices.

**Figure 4:** Serial Execution Times - K Neighbors Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.000008|0\.000008|0\.000009|
|1251|0\.003940|0\.003747|0\.004199|
|2851|0\.017226|0\.019429|0\.021668|
|4851|0\.049725|0\.056489|0\.062670|
|7251|0\.111905|0\.126558|0\.140568|
|10051|0\.216973|0\.245940|0\.271580|
|13251|0\.376010|0\.424689|0\.471096|
|16851|0\.603611|0\.684770|0\.759159|
|20851|0\.940527|1\.056135|1\.170458|
|25251|1\.393366|1\.567639|1\.729105|
|30051|2\.000402|2\.238927|2\.472483|
|35251|2\.625180|2\.973034|3\.298387|
|40851|3\.588093|4\.044826|4\.489815|
|46851|4\.768696|5\.378794|5\.941459|
|53251|6\.227153|6\.996702|7\.722056|
<br>

**Figure 5:** Serial Execution Times - Random Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.000003|0\.000009|0\.000014|
|1251|0\.002277|0\.005605|0\.002224|
|2851|0\.014341|0\.018742|0\.019854|
|4851|0\.013196|0\.079984|0\.057978|
|7251|0\.027327|0\.194853|0\.189936|
|10051|0\.286570|0\.323621|0\.429913|
|13251|0\.175853|0\.293719|0\.426890|
|16851|0\.281435|0\.485431|0\.697147|
|20851|1\.135531|1\.165203|0\.617124|
|25251|0\.349374|0\.607953|0\.897863|
|30051|0\.480963|2\.911970|2\.830980|
|35251|1\.243129|1\.188263|1\.762931|
|40851|4\.913150|3\.770531|4\.078628|
|46851|6\.103648|7\.718187|6\.881578|
|53251|7\.381782|6\.430583|12\.295214|
<br>

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.004.jpeg)

**Figure 6:** Serial performance on different graphs

In Figure 6, the execution of the K Neighbors graphs are more predictable than the Random graphs. We see a steadier increase in the runtime as the graph size and percentage increase. On the other hand, the Random graphs do not follow a similar pattern.

## 3.2 OpenMP

<a name="_page8_x72.00_y72.00"></a>Similar to the serial version, we initialize the required data structures before beginning with the graph traversal. For every dequeued vertex, we partition the loop across all threads to process simultaneously. Parallelizing the graph traversal allows for a faster search since more threads are more likely to find the target vertex, thus executing faster.

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.005.png)

**Figure 7:** OpenMP algorithm code snippet

## 3.2.1 Results

<a name="_page9_x72.00_y72.00"></a>We run our algorithms using 2, 4, and 8 threads to compare performance. Given the nature of the parallel algorithms, we will only compare results using the K Neighbors matrices. Running BFS in parallel on the Random graphs is less insightful than on the K Neighbors graphs. This difference is because it is difficult to predict the shortest path between the start and end vertex for randomly generated graphs.

**Figure 8:** OpenMP 2 Thread Execution Times - K Neighbors Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.000097|0\.000033|0\.000030|
|1251|0\.002864|0\.003105|0\.003241|
|2851|0\.012761|0\.014095|0\.014341|
|4851|0\.033272|0\.037288|0\.039494|
|7251|0\.073053|0\.081082|0\.086316|
|10051|0\.137825|0\.154684|0\.163631|
|13251|0\.235216|0\.264816|0\.282509|
|16851|0\.377132|0\.426045|0\.451266|
|20851|0\.578342|0\.648942|0\.693373|
|25251|0\.849481|0\.954961|1\.013081|
|30051|1\.210061|1\.354155|1\.437587|
|35251|1\.621143|1\.829512|1\.953152|
|40851|2\.193739|2\.473776|2\.632945|
|46851|2\.887255|3\.257075|3\.468164|
|53251|3\.745573|4\.205001|4\.467947|
<br>

**Figure 9:** OpenMP 4 Thread Execution Times - K Neighbors Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.000112|0\.000035|0\.000031|
|1251|0\.002116|0\.002243|0\.002287|
|2851|0\.009169|0\.009617|0\.009937|
|4851|0\.023638|0\.026042|0\.026653|
|7251|0\.051027|0\.056264|0\.057765|
|10051|0\.095353|0\.105794|0\.108831|
|13251|0\.162643|0\.179938|0\.185908|
|16851|0\.259841|0\.289912|0\.297803|
|20851|0\.395912|0\.439432|0\.452949|
|25251|0\.577453|0\.643377|0\.662627|
|30051|0\.820718|0\.904267|0\.934706|
|35251|1\.110595|1\.237724|1\.278742|
|40851|1\.491152|1\.655401|1\.716806|
|46851|1\.952449|2\.177430|2\.246953|
|53251|2\.528821|2\.806877|2\.898230|
<br>

**Figure 10:** OpenMP 8 Thread Execution Times - K Neighbors Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.0001676|0\.00003902|0\.00003327|
|1251|0\.00179119|0\.00160861|0\.00134756|
|2851|0\.00716616|0\.00684506|0\.00654966|
|4851|0\.01834379|0\.01772048|0\.01680154|
|7251|0\.03868788|0\.03675821|0\.03345834|
|10051|0\.07123136|0\.06874953|0\.06394442|
|13251|0\.12061736|0\.11624394|0\.10750707|
|16851|0\.19134513|0\.18416179|0\.18536508|
|20851|0\.29028584|0\.27760826|0\.26157968|
|25251|0\.42040914|0\.40657605|0\.37210953|
|30051|0\.59386981|0\.55861465|0\.51166484|
|35251|0\.81059679|0\.76397417|0\.6989779|
|40851|1\.08450677|1\.0182001|0\.92436699|
|46851|1\.41619088|1\.33788445|1\.21412099|
|53251|1\.83200852|1\.71929035|1\.54666316|
<br>

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.006.jpeg)

**Figure 11:** OpenMP speedup compared to serial version


Using two threads, we notice a 1.6x speedup over the serial version, the four threads runtime sees a speedup of ~2.5x, and the eight threads average a speedup of 4x. The speedup is more significant for graphs with 20% edges. In the case of “OMP 8, 20%”, there is a 5x speedup. As we increase the number of edges per thread, parallel implementations benefit from faster graph traversal.

## <a name="_page11_x72.00_y498.95"></a>3.3 PThreads

The PThreads implementation is more efficient than the OpenMP implementation because it allows for more fine-grained control over the shared resources and synchronization between threads. In the pthreads implementation, a mutex controls access to the shared queue and the shared "found" variable that indicates the found target vertex. Each thread acquires the mutex, dequeues a vertex, processes its neighbors, and updates the shared data structures in a critical section protected by the mutex. This approach ensures that only one thread can access the shared data structures at a time, eliminating the possibility of data races and other synchronization issues.

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.007.png)

**Figure 12:** PThreads algorithm code snippet

<br>

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.008.png)

**Figure 13:** PThreads thread algorithm code snippet

## 3.3.1 Results

<a name="_page14_x72.00_y72.00"></a>**Figure 14:** 2 PThreads Execution Times - K Neighbors Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.000179|0\.000100|0\.000031|
|1251|0\.002324|0\.002459|0\.002601|
|2851|0\.009971|0\.010658|0\.010981|
|4851|0\.027809|0\.029935|0\.031339|
|7251|0\.062251|0\.066621|0\.069955|
|10051|0\.120617|0\.129765|0\.135211|
|13251|0\.208679|0\.445439|0\.234526|
|16851|0\.334904|0\.359484|0\.377585|
|20851|0\.520268|0\.556250|0\.583267|
|25251|0\.774631|0\.826316|0\.860847|
|30051|1\.112487|1\.177821|1\.228594|
|35251|1\.457018|1\.561673|1\.641045|
|40851|1\.992387|2\.133023|2\.236831|
|46851|2\.654970|2\.847780|2\.973488|
|53251|3\.468844|3\.696212|3\.863485|
<br>

**Figure 15:** 4 PThreads Execution Times - K Neighbors Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.000235|0\.000089|0\.000075|
|1251|0\.001280|0\.001364|0\.001423|
|2851|0\.005191|0\.005520|0\.005801|
|4851|0\.014614|0\.015681|0\.016381|
|7251|0\.032754|0\.034680|0\.036654|
|10051|0\.062653|0\.067250|0\.070670|
|13251|0\.108084|0\.116603|0\.122904|
|16851|0\.173487|0\.186903|0\.197885|
|20851|0\.269349|0\.289589|0\.305370|
|25251|0\.400181|0\.429162|0\.450625|
|30051|0\.576294|0\.613273|0\.640839|
|35251|0\.751187|0\.809651|0\.855045|
|40851|1\.366011|1\.108997|1\.165143|
|46851|1\.372390|1\.473441|1\.561551|
|53251|1\.793777|1\.921999|2\.023460|
<br>

**Figure 16:** 8 PThreads Execution Times - K Neighbors Graphs

|row\_length|10%|15%|20%|
| - | - | - | - |
|51|0\.000425|0\.000130|0\.000118|
|1251|0\.000791|0\.000791|0\.000816|
|2851|0\.002947|0\.002955|0\.003259|
|4851|0\.007849|0\.008544|0\.009136|
|7251|0\.022205|0\.019009|0\.020046|
|10051|0\.033873|0\.036487|0\.038306|
|13251|0\.059493|0\.063064|0\.067102|
|16851|0\.093443|0\.100494|0\.106916|
|20851|0\.145000|0\.235036|0\.165642|
|25251|0\.215409|0\.230793|0\.244245|
|30051|0\.310457|0\.330551|0\.346514|
|35251|0\.403130|0\.435790|0\.462543|
|40851|0\.553410|0\.597856|0\.632699|
|46851|0\.736799|0\.793448|0\.914195|
|53251|0\.962783|1\.038433|1\.093599|
<br>

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.009.jpeg)

**Figure 17:** OpenMP speedup compared to serial version

Overall, the PThreads implementation achieved better performance across all numbers of threads. But we notice certain performance drops at seemingly random row sizes.

# <a name="_page16_x72.00_y449.84"></a>4 GPU

Creating a GPU program follows a standard procedure: allocating device memory, copying host data to the device, running the kernel, and transferring results back to the host. The biggest challenges we encountered when implementing a breadth-first search on a GPU were mapping the graph data to the device, partitioning the graph data to process, and returning a boolean value once we found the target node we were searching for.

## 4.1 Memory<a name="_page16_x72.00_y567.70"></a> Mapping

As discussed in section one, graph challenges, representing the graph data structure on GPU is challenging. First, although it is possible to allocate data for a 2-dimensional array, it could be more efficient for the GPU to process. To overcome this, we tried converting the 2-dimensional array into a 1-dimensional one. However, this led to the next challenge of partitioning the work for the GPU to process effectively to benefit from GPU power.

## 4.2 Partitioning<a name="_page17_x72.00_y72.00"></a> Data

Partitioning the data for processing by the GPU is challenging because of the sparseness of the graph. Furthermore, breadth-first search parallelization is complex because the memory accesses and work distribution are irregular and data-dependent. Load-balancing is prevalent when some threads have more work than others, which can happen due to the nature of breadth-first search traversing the graph's structure. In addition, each subsequent layer of explored nodes is dependent on the previous nodes, which can be further complicated when multiple threads traverse the same node. To overcome this challenge, we have to devise a way to use the GPU for processing layers of nodes while using the CPU as a control for calling the kernels to run.

## 4.3 Running<a name="_page17_x72.00_y234.69"></a> the Kernel

To solve the problem of thread utilization, load balancing, and implementation of the breadth-first search algorithm, we tried to use multiple kernels for different parts of the algorithm. For example, one kernel would explore the nodes in the queue, and another would track if we found the target node.

The next challenge with running the kernels was keeping track of the queue for neighbor exploration by the threads. We tried different implementations from the literature on the GPU's breadth-first search. One implementation we tried was using frontier queues that would keep track of the nodes explored by each thread. Although we tried to implement this in the Cuda code, we ran back into the challenge of devising a method to allocate the memory space to the GPU for the queues.

## 4.4 Discussion

<a name="_page17_x72.00_y427.24"></a>After reading much of the literature on implementing breadth-first search on a GPU, it became clear that this is a challenge many researchers have been trying to solve and are still solving. Unless graphs are structured and optimized for processing on a GPU, it is challenging to justify implementation if there is more work but less value in that work. There are benefits to implementing graph algorithms on a GPU. Still, the amount of work necessary to structure the program and partition the data processing requires more expertise in parallel algorithms, mainly parallel graph algorithms.

In conclusion, we still need to get the GPU code to implement breadth-first search correctly, but we learned a great deal about some of the challenges of using GPUs for specific tasks.

# 5. Closing<a name="_page17_x72.00_y619.80"></a> Thoughts

In conclusion, the breadth-first search algorithm is helpful for many applications. With the appropriate implementation to partition and process the nodes of the graph structure, it is worthwhile to parallelize. However, implementing the algorithm for processing on a GPU is challenging due to the problem's nature and the GPU's architecture. Furthermore, using openMP and multithreading, we see significant improvement in processing times. This processing time demonstrates the benefits of parallel processing breadth-first search. However, understanding how to partition the graph data structure to make the work from threads meaningful is a challenge that takes time to solve. Given an extended time for this project, we believe we could have figured out how to implement the graph algorithm on a GPU and partition the work meaningfully.

# 6. Compilation<a name="_page18_x72.00_y167.21"></a> Instructions

To test the code and generate data run the following command:

```
make clean && make && ./bfs
```

# 7. Program<a name="_page18_x72.00_y249.17"></a> Structure

![](Aspose.Words.51f4ceca-6222-4fd4-9e6f-b96533b225cf.010.png)

**Figure 18:** Project folder structure
