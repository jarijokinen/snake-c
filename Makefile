TARGET=snake

CC=gcc
RM=rm
CFLAGS=-Wall
LIBS=-lncurses
OBJS=snake.o

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< $(LIBS) -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIBS) -o $@

all: $(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
