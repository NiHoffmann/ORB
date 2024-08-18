SRC_USERMOD_C += $(USERMOD_DIR)/MonitorModule.c
SRC_USERMOD_C += $(USERMOD_DIR)/KeyConstants.c

# Add our module directory to the include path.
CFLAGS_USERMOD += -I$(USERMOD_DIR)

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += -lstdc++