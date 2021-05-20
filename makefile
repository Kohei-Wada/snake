OBJECTS = main.o game.o list.o ui.o getch.o snake.o
CFLAGS = -g -Wall 
LDLIBS = -pthread 
INCLUDE = -I include 
TARGET = snake
SOURCES = $(OBJECTS: %c = %o)

LIBDIR = lib
OBJDIR = obj
SOURCEDIR = src


#$(warning $(SOURCES))
bin/$(TARGET) : $(addprefix obj/, $(notdir $(OBJECTS))) 
	$(CC) $(CFLAGS)  -o $@ $^  $(LDLIBS)

$(OBJDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS) $(LDLIBS) $(INCLUDE) -o $@ -c $< 

$(OBJDIR)/%.o : $(LIBDIR)/%.c
	$(CC) $(CFLAGS) $(LDLIBS) $(INCLUDE) -o $@ -c $<
clean :
	rm -f bin/$(TARGET) $(OBJDIR)/*
