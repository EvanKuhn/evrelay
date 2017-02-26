# Compilation
CC      = clang-3.8
CFLAGS  = -c -Wall --std=c99
LDFLAGS =

# Directories
INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
LIB_DIR = ./lib
EXT_DIR = ./ext
BIN_DIR = ./bin

# External library versions
LIBUV_VERSION = 1.11.0

SOURCES := $(shell find $(SRC_DIR) -name '*.c')
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

#-------------------------------------------------------------------------------
# Build evrelay
#-------------------------------------------------------------------------------

all: evrelay

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@ -I$(INC_DIR)

evrelay: $(OBJECTS) libuv
	$(CC) -o $(BIN_DIR)/$@ $(LDFLAGS) -I$(INC_DIR) $(OBJECTS)

#-------------------------------------------------------------------------------
# Build libuv
#-------------------------------------------------------------------------------

.PHONY: libuv
libuv: $(LIB_DIR)/libuv/include $(LIB_DIR)/libuv/libuv.a

$(LIB_DIR)/libuv/include: $(LIB_DIR)/libuv/ $(EXT_DIR)/libuv-$(LIBUV_VERSION)
	ln -fs $(EXT_DIR)/libuv-$(LIBUV_VERSION)/include $(LIB_DIR)/libuv/include

$(LIB_DIR)/libuv/libuv.a: $(LIB_DIR)/libuv/ $(EXT_DIR)/libuv-$(LIBUV_VERSION)/.libs/libuv.a
	ln -fs $(EXT_DIR)/libuv-$(LIBUV_VERSION)/.libs/libuv.a $(LIB_DIR)/libuv/libuv.a

$(LIB_DIR)/libuv/:
	mkdir -p $(LIB_DIR)/libuv/

$(EXT_DIR)/libuv-$(LIBUV_VERSION)/.libs/libuv.a: $(EXT_DIR)/libuv-$(LIBUV_VERSION)
	cd $(EXT_DIR)/libuv-$(LIBUV_VERSION) && sh autogen.sh && ./configure && make

$(EXT_DIR)/libuv-$(LIBUV_VERSION):
	tar -xzf $(EXT_DIR)/tarballs/libuv-$(LIBUV_VERSION).tar.gz -C $(EXT_DIR)

#-------------------------------------------------------------------------------
# Clean
#-------------------------------------------------------------------------------

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*

.PHONY: libclean
libclean:
	rm -rf $(EXT_DIR)/libuv*
	rm -rf $(LIB_DIR)/libuv
