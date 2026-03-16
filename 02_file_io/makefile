CC = gcc
CFLAGS = -g -Wall
TARGET = fdflag.out
OBJS = fdflag.o

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

fdflag.o: fdflag.c
