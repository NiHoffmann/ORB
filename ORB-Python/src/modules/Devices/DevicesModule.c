// Include MicroPython API.
#include "py/runtime.h"
#include "Devices/Motor/MotorModule.h"
#include "Devices/Servo/ServoModule.h"
#include "Devices/Sensor/SensorModule.h"
#include "Devices/Button/ButtonType.h"
#include "Devices/EV3Ultrasonic/EV3UltrasonicType.h"
#include "Devices/EV3Touch/EV3TouchType.h"

static mp_obj_t devices___init__(void) {
    if (!MP_STATE_VM(devices_initialised)) {
        MP_STATE_VM(devices_initialised) = true;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(devices___init___obj, devices___init__);
MP_REGISTER_ROOT_POINTER(int devices_initialised);

static const mp_rom_map_elem_t devices_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_devices) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&devices___init___obj) },

    { MP_ROM_QSTR(MP_QSTR_motor),  MP_ROM_PTR(&motor_type) },
    { MP_ROM_QSTR(MP_QSTR_servo),  MP_ROM_PTR(&servo_type) },
	{ MP_ROM_QSTR(MP_QSTR_sensor),  MP_ROM_PTR(&sensor_type) },
	{ MP_ROM_QSTR(MP_QSTR_button),  MP_ROM_PTR(&button_type) },

	{ MP_ROM_QSTR(MP_QSTR_EV3Ultrasonic),  MP_ROM_PTR(&ev3_ultrasonic_type) },
	{ MP_ROM_QSTR(MP_QSTR_EV3Touch),  MP_ROM_PTR(&ev3_touch_type) },
};
static MP_DEFINE_CONST_DICT(devices_globals, devices_globals_table);

const mp_obj_module_t devices_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&devices_globals,
};

MP_REGISTER_MODULE(MP_QSTR_devices, devices_module);
