CC = g++
CFLAGS = -std=c++20 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

DEPS = map.hpp player.hpp game.hpp attack.hpp enemy.hpp SpatialHash.hpp

OBJECTS = main.o map.o player.o game.o attack.o enemy.o SpatialHash.o

PROGRAM = vampire

.PHONY: all clean lint

all: $(PROGRAM)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm -f *.o *.a $(PROGRAM)
