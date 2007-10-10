CC = gcc
LIBS = `sdl-config --cflags --libs` -lSDL_image -lGLU -lGL
OBJS = element.o  graphics.o  grid.o  main.o  particle.o  psys.o
FLAGS = -Wall -O3 -std=c99 -s
all: unix_fluid
unix_fluid: $(OBJS)
	$(CC) $(LIBS) -o $@ $(OBJS) $(FLAGS)
%.o:%.c
	$(CC) -c $+ -o $@ $(FLAGS)
clean:
	rm -f *.o *~
