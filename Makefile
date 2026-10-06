CC = g++
CFLAGS = -std=c++20 -Wall -Werror -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

DEPS = config.hpp map.hpp player.hpp game.hpp attack.hpp enemy.hpp SpatialHash.hpp SpawnManager.hpp

OBJECTS = main.o map.o player.o game.o attack.o enemy.o SpatialHash.o SpawnManager.o

PROGRAM  = survivor

.PHONY: all clean lint

all: $(PROGRAM)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm -f *.o *.a $(PROGRAM)
