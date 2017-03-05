PROJ_ROOT = .
include $(PROJ_ROOT)/Makefile.common

LIBS = $(LIBUV_LIB_FILE) $(LIBYAML_LIB_FILE)

#-------------------------------------------------------------------------------
# Build evrelay
#-------------------------------------------------------------------------------

all: evrelay

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c libuv
	$(CC) $(CFLAGS) -c $< -o $@ $(INC_FLAGS)

evrelay: $(OBJECTS) libuv
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$@ $(INC_FLAGS) $(OBJECTS) $(LIBS)

scratch: $(OBJECTS) libyaml
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
# Build libyaml
#-------------------------------------------------------------------------------

.PHONY: libyaml
libyaml: $(LIBYAML_LIB_FILE)

$(LIBYAML_LIB_FILE): $(LIBYAML_PROJ_DIR)
	cd $(LIBYAML_PROJ_DIR) && ./bootstrap && ./configure && make

$(LIBYAML_PROJ_DIR):
	tar -xzf $(LIBYAML_TAR_FILE) -C $(EXT_DIR)

#-------------------------------------------------------------------------------
# Clean
#-------------------------------------------------------------------------------

.PHONY: clean
clean:
	find . -type f | xargs -n 5 touch # To fix 'clock skew' warning on vagrant
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/*

.PHONY: libclean
libclean:
	rm -rf $(EXT_DIR)/libuv*
	rm -rf $(EXT_DIR)/libyaml*
