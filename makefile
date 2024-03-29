OBJECTS = main.o game.o list.o ui.o getch.o snake.o player.o board.o observer.o enemy.o
CFLAGS = -g -Wall 
LDLIBS = #-pthread 
INCLUDE = -I include 
TARGET = snake
SOURCES = $(OBJECTS: %c = %o)

LIBDIR = lib
OBJDIR = obj
SOURCEDIR = src
BINDIR = bin
CC = gcc



all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET) : $(addprefix obj/, $(notdir $(OBJECTS))) 
	@if [ ! -d $(BINDIR) ]; then\
		mkdir $(BINDIR);\
	fi
	$(CC) $(CFLAGS)  -o $@ $^  $(LDLIBS)

$(OBJDIR)/%.o : $(SOURCEDIR)/%.c
	@if [ ! -d $(OBJDIR) ]; then\
		mkdir $(OBJDIR);\
	fi;
	$(CC) $(CFLAGS) $(LDLIBS) $(INCLUDE) -o $@ -c $< 

$(OBJDIR)/%.o : $(LIBDIR)/%.c
	@if [ ! -d $(OBJDIR) ]; then\
		mkdir $(OBJDIR);\
	fi;
	$(CC) $(CFLAGS) $(LDLIBS) $(INCLUDE) -o $@ -c $<
	
clean :
	rm -f bin/$(TARGET) $(OBJDIR)/*
