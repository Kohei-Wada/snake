
OBJS=snake.o
SRCS=$(OBJS:%.0=%.c)
CFLAGS=-g -Wall
LDLIBS=
TARGET=snake
$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)
