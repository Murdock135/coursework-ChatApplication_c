# Makefile for Chat Application

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Source files
SERVER_SRC = src/server.c src/users.c
CLIENT_SRC = src/client.c

# Object files
SERVER_OBJ = $(SERVER_SRC:src/%.c=build/%.o)
CLIENT_OBJ = $(CLIENT_SRC:src/%.c=build/%.o)

# Targets
SERVER = build/server
CLIENT = build/client

# Default target
all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJ) | build
	$(CC) $(SERVER_OBJ) -o $@

$(CLIENT): $(CLIENT_OBJ) | build
	$(CC) $(CLIENT_OBJ) -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

.PHONY: all clean
