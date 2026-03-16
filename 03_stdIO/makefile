CC = gcc
CFLAGS = -g -Wall
TARGET = stringIO.out
OBJS = stringIO.o

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

stringIO.o: stringIO.c
