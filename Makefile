CC         = clang-3.8
CFLAGS     = -c -Wall --std=c99
LDFLAGS    =
EXECUTABLE = evrelay

INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
LIB_DIR = ./lib
BIN_DIR = ./bin

SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Rule to build object files for corresponding C files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@ -I$(INC_DIR)

evrelay: $(OBJECTS)
	$(CC) -o $(BIN_DIR)/$@ $(LDFLAGS) -I$(INC_DIR) $(OBJECTS)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*
