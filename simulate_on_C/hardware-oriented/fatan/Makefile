CC = gcc
CFLAGS = -Wall -O3
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)


TARGET = test

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

clean:
	rm $(TARGET) $(OBJS) *~
