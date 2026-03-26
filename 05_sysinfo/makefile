CC = gcc
CFLAGS = -g -Wall
TARGET = uid.out
OBJS = uid.o

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

uid.o: uid.c
