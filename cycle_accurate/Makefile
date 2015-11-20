CC = gcc
CFLAGS = -Wall -O3
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)


TARGET = sim

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -Xlinker --warn-common

clean:
	rm $(TARGET) $(OBJS) *~
