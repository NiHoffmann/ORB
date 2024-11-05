SRC_USERMOD_C += $(USERMOD_DIR)/TimeModule.c

# Add our module directory to the include path.
CFLAGS_USERMOD += -I$(USERMOD_DIR)

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += -lstdc++