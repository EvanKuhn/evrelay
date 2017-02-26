PROJ_ROOT = .
include $(PROJ_ROOT)/Makefile.common

#-------------------------------------------------------------------------------
# Build evrelay
#-------------------------------------------------------------------------------

all: evrelay

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c libuv
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_FLAGS)

evrelay: $(OBJECTS) libuv
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$@ $(INC_FLAGS) $(OBJECTS) $(LIBUV_LIB_FILE)

scratch: $(OBJECTS)
	$(MAKE) -C test/scratch

#-------------------------------------------------------------------------------
# Build libuv
#-------------------------------------------------------------------------------

.PHONY: libuv
libuv: $(LIBUV_LIB_FILE)

$(LIBUV_LIB_FILE): $(LIBUV_PROJ_DIR)
	cd $(LIBUV_PROJ_DIR) && sh autogen.sh && ./configure && make

$(LIBUV_PROJ_DIR):
	tar -xzf $(LIBUV_TAR_FILE) -C $(EXT_DIR)

#-------------------------------------------------------------------------------
# Clean
#-------------------------------------------------------------------------------

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*

.PHONY: libclean
libclean:
	rm -rf $(EXT_DIR)/libuv*
