CC = gcc
LIBS = `sdl-config --cflags --libs` -lSDL_ttf -lGLU -lGL
OBJS = element.o  graphics.o  grid.o  main.o  particle.o  psys.o config.o gui.o
FLAGS = -Wall -O3 -std=c99
all: unix_fluid
unix_fluid: $(OBJS)
	$(CC) $(LIBS) -o $@ $(OBJS) $(FLAGS)
%.o:%.c
	$(CC) -c $+ -o $@ $(FLAGS)
clean:
	rm -f *.o *~
