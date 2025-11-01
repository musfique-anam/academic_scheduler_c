CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS = main.c input.c assign.c scheduler.c conflict.c exam_scheduler.c globals.c
OBJS = $(SRCS:.c=.o)
TARGET = scheduler.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
