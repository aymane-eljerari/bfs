CC=gcc
CFLAGS=-Wall -pedantic

SRCS=main.c graph.c queue.c bfs.c
OBJS=$(SRCS:.c=.o)
EXEC=bfs

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)