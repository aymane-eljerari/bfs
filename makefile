CC=gcc
CFLAGS=-Wall
OMPFLAG="-I/opt/homebrew/opt/libomp/include"

SOURCES=graph.c queue.c bfs.c omp.c pthreads.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bfs

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OMPFLAG) -fopenmp $(CFLAGS) main.c $(OBJECTS) -o $@

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c -o queue.o

graph.o: graph.c 
	$(CC) $(CFLAGS) -c graph.c -o graph.o

bfs.o: bfs.c graph.c queue.c
	$(CC) $(CFLAGS) -c bfs.c -o bfs.o

omp.o: omp.c graph.c queue.c
	$(CC) $(OMPFLAG) -fopenmp $(CFLAGS) -c omp.c -o omp.o

pthreads.o: pthreads.c graph.c queue.c
	$(CC) $(CFLAGS) -c pthreads.c -o pthreads.o

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

