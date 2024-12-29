# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*/*.c) $(SRC_DIR)/firewall.c
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Target executable
TARGET = firewall

# Default rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: clean
