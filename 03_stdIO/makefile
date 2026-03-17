CC = gcc
CFLAGS = -g -Wall
TARGET = student_info.out
OBJS = student_info.o

all: $(TARGET)

clean:
	rm -f *.o
	rm -f $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

student_info.o: student_info.c
