#include "NXTLightSensor.h"
#include "Devices/Sensor/SensorModule.h"
#include "helper.h"
#include "Devices/Sensor/SensorModule.h"

const mp_obj_type_t nxt_light_sensor_type;

typedef struct _nxt_light_sensor_obj_t {
    mp_obj_base_t base;
    sensor_obj_t sensor;
} nxt_light_sensor_obj_t;

static void mp_nxt_light_sensor_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    nxt_light_sensor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "nxt_light_sensor(%d)", self->sensor.port);
}

static mp_obj_t mp_nxt_light_sensor_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_port};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_port,         MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1 } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int port =   args[ARG_port].u_int;
    CHECK_VALID_PORT(port, sensor_obj_list);

    nxt_light_sensor_obj_t *self = mp_obj_malloc(nxt_light_sensor_obj_t, type);

    sensor_obj_t sensor = sensor_obj_list[port];
    sensor.type      = UART;
    sensor.mode      = 0;
    sensor.mem_offset= 0;

    configSensor(sensor.port, sensor.type, sensor.mode, sensor.mem_offset);
    self->sensor = sensor;
    return self;
}

static mp_obj_t nxt_light_sensor_get_func(mp_obj_t self_in) {
    nxt_light_sensor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    sensor_obj_t sensor = self->sensor;
    sensor_return_values *ret = getSensor(sensor.port);

    int actual_interrations = 49;
    float val = 0;
    float filter_value = 0;
    float current_val = 0
    int spike = 0;
    int dip = 0;
    float coeffitient = 0.075;
    float average_value = 0;
    float gain = 0;
    float offset = 0;


    for(int i=0;i<49;i++){
        *ret = getSensor(sensor.port);
        current_val = ret->value[0] & 0xFFFF;

        if(current_val > spike || current_val < dip){
            actual_interrations--;
            continue;
        }

        filter_value = coeffitient * current_val + (1 - coeffitient) * filter_value;
        val += filter_value;
    }

    average_value = val/actual_interrations;

    return mp_obj_new_int(gain * (average_value- offset));
}
static MP_DEFINE_CONST_FUN_OBJ_1(nxt_light_sensor_get_func_obj, nxt_light_sensor_get_func);

static const mp_rom_map_elem_t nxt_light_sensor_local_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&nxt_light_sensor_get_func_obj) },
};
static MP_DEFINE_CONST_DICT(nxt_light_sensor_local, nxt_light_sensor_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    nxt_light_sensor_type,
    MP_QSTR_EV3Ultrasonic,
    MP_TYPE_FLAG_NONE,
    make_new, mp_nxt_light_sensor_make_new,
    print, mp_nxt_light_sensor_print,
    locals_dict, &nxt_light_sensor_local
);

