# This file is part of the MicroPython project, http://micropython.org/
# The MIT License (MIT)
# Copyright (c) 2022-2023 Damien P. George

# Set the location of the top of the MicroPython repository.
MICROPYTHON_TOP = ../../../micropython

# Add C modules.
USER_C_MODULES = ../../src/modules
CFLAGS += -I../../src/modules
CFLAGS += -I../../src/c_interface/mockups
CFLAGS += -I../../src/python-vm


QSTR_DEFS += ../../src/modules/qstr.h
# Include the main makefile fragment to build the MicroPython component.
include $(MICROPYTHON_TOP)/ports/embed/embed.mk
