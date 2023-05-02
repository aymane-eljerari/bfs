# CC=gcc
# CFLAGS=-Wall -pedantic

# SRCS=main.c graph.c queue.c bfs.c
# OBJS=$(SRCS:.c=.o)
# EXEC=out

# all: $(EXEC)

# $(EXEC): $(OBJS)
# 	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJS) $(EXEC)



# CC=gcc
# CFLAGS=-Wall
# OMPFLAG="-I/opt/homebrew/opt/libomp/include"

# SOURCES=main.c graph.c queue.c bfs.c 
# OBJECTS=$(SOURCES:.c=.o)
# EXECUTABLE=out


# all: $(SOURCES) $(EXECUTABLE)

# $(EXECUTABLE): $(OBJECTS)
# 	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# .c.o:
# 	$(CC) $(CFLAGS) -c $< -o $@

# bfs.o: bfs.c 
# 	$(CC) $(OMPFLAG) $(CFLAGS) -c bfs.c -o bfs.o

# clean:
# 	rm -f $(OBJECTS) $(EXECUTABLE)

CC=gcc
CFLAGS=-Wall
OMPFLAG="-I/opt/homebrew/opt/libomp/include"

SOURCES=graph.c queue.c bfs.c omp.c
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
	$(CC) $(OMPFLAG) -fopenmp $(CFLAGS) -c bfs.c -o bfs.o

omp.o: omp.c graph.c queue.c
	$(CC) $(OMPFLAG) -fopenmp $(CFLAGS) -c omp.c -o omp.o

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

