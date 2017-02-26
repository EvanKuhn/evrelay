PROJ_ROOT = ../..
include $(PROJ_ROOT)/Makefile.common

all: $(BIN_DIR)/test_hash $(BIN_DIR)/test_yaml

$(BIN_DIR)/test_hash: test_hash.c
	$(CC) $(CFLAGS) -o $@ $< $(INC_FLAGS) $(OBJ_DIR)/hash.o

$(BIN_DIR)/test_yaml: test_yaml.c
	$(CC) $(CFLAGS) -o $@ $< $(INC_FLAGS) $(LIBYAML_LIB_FILE)
