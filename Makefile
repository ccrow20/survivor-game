CC = g++
CFLAGS = -std=c++20 -Wall -Werror -pedantic -g -Iinclude
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
DEPS = $(wildcard $(INC_DIR)/*.hpp)

PROGRAM = $(BIN_DIR)/survivor

.PHONY: all clean lint

all: $(PROGRAM)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.a $(PROGRAM)
