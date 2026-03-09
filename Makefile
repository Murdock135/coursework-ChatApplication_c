# Example Makefile structure for a C project:

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.c)

# Object files generated from source files, stored in the build directory
# Target executable: build/myapp
OBJ = $(SRC:src/%.c=build/%.o)
TARGET = build/myapp

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build
