CC = gcc
CFLAGS = -g -Wall
TARGET = realpath.out
OBJS = realpath.o

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

realpath.o: realpath.c
