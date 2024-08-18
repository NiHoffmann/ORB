#include "EV3UltrasonicType.h"
#include "Sensor.h"
#include "helper.h"
#include "Devices/Sensor/SensorModule.h"

const mp_obj_type_t ev3_ultrasonic_type;

typedef struct _ev3_ultrasonic_obj_t {
    mp_obj_base_t base;
    sensor_obj_t sensor;
} ev3_ultrasonic_obj_t;

static void mp_ev3_ultrasonic_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    ev3_ultrasonic_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "ev3_ultrasonic(%d)", self->sensor.port);
}

static mp_obj_t mp_ev3_ultrasonic_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_port};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_port,         MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1 } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int port =   args[ARG_port].u_int;
    CHECK_VALID_PORT(port, sensor_obj_list);

    ev3_ultrasonic_obj_t *self = mp_obj_malloc(ev3_ultrasonic_obj_t, type);

    sensor_obj_t sensor = sensor_obj_list[port];
    sensor.type      = UART;
    sensor.mode      = 0;
    sensor.mem_offset= 0;

    configSensor(sensor.port, sensor.type, sensor.mode, sensor.mem_offset);
    self->sensor = sensor;
    return self;
}

static mp_obj_t ev3_ultrasonic_get_func(mp_obj_t self_in) {
    ev3_ultrasonic_obj_t *self = MP_OBJ_TO_PTR(self_in);
    sensor_obj_t sensor = self->sensor;
    sensor_return_values *ret = getSensor(sensor.port);

    return mp_obj_new_int(ret->value[0] & 0xFFFF );
}
static MP_DEFINE_CONST_FUN_OBJ_1(ev3_ultrasonic_get_func_obj, ev3_ultrasonic_get_func);

static const mp_rom_map_elem_t ev3_ultrasonic_local_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&ev3_ultrasonic_get_func_obj) },
};
static MP_DEFINE_CONST_DICT(ev3_ultrasonic_local, ev3_ultrasonic_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    ev3_ultrasonic_type,
    MP_QSTR_EV3Ultrasonic,
    MP_TYPE_FLAG_NONE,
    make_new, mp_ev3_ultrasonic_make_new,
    print, mp_ev3_ultrasonic_print,
    locals_dict, &ev3_ultrasonic_local
);

