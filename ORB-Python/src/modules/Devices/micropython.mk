SRC_USERMOD_C += $(USERMOD_DIR)/DevicesModule.c
SRC_USERMOD_C += $(USERMOD_DIR)/Motor/MotorModule.c
SRC_USERMOD_C += $(USERMOD_DIR)/Servo/ServoModule.c
SRC_USERMOD_C += $(USERMOD_DIR)/Sensor/SensorModule.c
SRC_USERMOD_C += $(USERMOD_DIR)/Button/ButtonType.c

SRC_USERMOD_C += $(USERMOD_DIR)/EV3Ultrasonic/EV3UltrasonicType.c
SRC_USERMOD_C += $(USERMOD_DIR)/EV3Touch/EV3TouchType.c


# Add our module directory to the include path.
CFLAGS_USERMOD += -I$(USERMOD_DIR)

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += -lstdc++