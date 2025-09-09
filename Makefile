CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g
SRC_DIR = src
OBJ_DIR = build
BIN = $(OBJ_DIR)/editor

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(BIN)

$(BIN): $(OBJ_FILES)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

run: all
	./$(BIN)

.PHONY: all clean run
