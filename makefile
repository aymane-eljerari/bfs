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

CC=gcc
CFLAGS=

SOURCES=graph.c queue.c bfs.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

bfs.o: bfs.c
	$(CC) $(CFLAGS) -fopenmp -c bfs.c -o bfs.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)