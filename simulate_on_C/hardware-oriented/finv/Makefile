CC = gcc
CFLAGS = -Wall -O3
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)


TARGET = test

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ invrom.h -lm

clean:
	rm $(TARGET) $(OBJS) *~
