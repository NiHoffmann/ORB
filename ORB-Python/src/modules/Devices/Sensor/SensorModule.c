#include "SensorModule.h"
#include "Sensor.h"
#include "helper.h"

const mp_obj_type_t sensor_type;

sensor_obj_t sensor_obj_list[4] = {
    { .base = { .type = &sensor_type }, .port = 0, .type = 0, .mode = 0, .mem_offset = 0 },
    { .base = { .type = &sensor_type }, .port = 1, .type = 0, .mode = 0, .mem_offset = 0 },
    { .base = { .type = &sensor_type }, .port = 2, .type = 0, .mode = 0, .mem_offset = 0 },
    { .base = { .type = &sensor_type }, .port = 3, .type = 0, .mode = 0, .mem_offset = 0 }
};

static void mp_sensor_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    sensor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "Sensor(%d)", self->port);
}

static mp_obj_t mp_sensor_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args){
    enum { ARG_port, ARG_type, ARG_mode, ARG_mem_offset};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_port,         MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1 } },
        { MP_QSTR_type,         MP_ARG_INT, {.u_int = 0 } },
        { MP_QSTR_mode,         MP_ARG_INT, {.u_int = 0 } },
        { MP_QSTR_mem_offset,   MP_ARG_INT, {.u_int = 0 } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int port =   args[ARG_port].u_int;

    CHECK_VALID_PORT(port, sensor_obj_list);

    sensor_obj_t *self = &sensor_obj_list[port];

    self->type      = args[ARG_type].u_int;
    self->mode      = args[ARG_mode].u_int;
    self->mem_offset= args[ARG_mem_offset].u_int;

    configSensor(self->port, self->type, self->mode, self->mem_offset);
    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t get(mp_obj_t self_in) {
    sensor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    sensor_return_values *ret = getSensor(self->port);

    mp_obj_t dict = mp_obj_new_dict(4);

    mp_obj_t values = mp_obj_new_list(0, NULL);
    mp_obj_list_append(values, mp_obj_new_int(ret->value[0]));
    mp_obj_list_append(values, mp_obj_new_int(ret->value[1]));

    mp_obj_dict_store(dict, mp_obj_new_str("values", 6), values);
    mp_obj_dict_store(dict, mp_obj_new_str("type", 4), mp_obj_new_int(ret->type));
    mp_obj_dict_store(dict, mp_obj_new_str("option", 6), mp_obj_new_int(ret->option));
    mp_obj_dict_store(dict, mp_obj_new_str("lenExp", 6), mp_obj_new_int(ret->lenExp));

    return dict;
}
static MP_DEFINE_CONST_FUN_OBJ_1(get_obj, get);

static mp_obj_t get_digital(mp_obj_t self_in) {
    sensor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(getSensorDigital(self->port));
}
static MP_DEFINE_CONST_FUN_OBJ_1(get_digital_obj, get_digital);

static mp_obj_t get_value_ext(mp_obj_t self_in, mp_obj_t ch) {
    sensor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_int(getSensorValueExt(self->port, (ch)));
}
static MP_DEFINE_CONST_FUN_OBJ_2(get_value_ext_obj, get_value_ext);

static mp_obj_t config(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args){
    enum { ARG_type, ARG_mode, ARG_mem_offset};

    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_type,                 MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
        { MP_QSTR_mode,                 MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL } },
        { MP_QSTR_mem_offset,           MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1  , pos_args + 1 , kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    sensor_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);

    ACCEPT_VALUE_KW_GIVEN(ARG_type, self->type, mp_obj_get_int);
    ACCEPT_VALUE_KW_GIVEN(ARG_mode, self->mode, mp_obj_get_int);
    ACCEPT_VALUE_KW_GIVEN(ARG_mem_offset, self->mem_offset , mp_obj_get_int);

    configSensor(self->port,self->type,self->mode,self->mem_offset);

    return MP_OBJ_FROM_PTR(self);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(config_obj, 0, config);


static const mp_rom_map_elem_t sensor_local_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&get_obj) },
    { MP_ROM_QSTR(MP_QSTR_getDigital), MP_ROM_PTR(&get_digital_obj) },
    { MP_ROM_QSTR(MP_QSTR_getValueExt), MP_ROM_PTR(&get_value_ext_obj) },
    { MP_ROM_QSTR(MP_QSTR_config), MP_ROM_PTR(&config_obj) },

    { MP_ROM_QSTR(MP_QSTR_S1) , MP_ROM_INT(S1)},
    { MP_ROM_QSTR(MP_QSTR_S2) , MP_ROM_INT(S2)},
    { MP_ROM_QSTR(MP_QSTR_S3) , MP_ROM_INT(S3)},
    { MP_ROM_QSTR(MP_QSTR_S4) , MP_ROM_INT(S4)},

    { MP_ROM_QSTR(MP_QSTR_Analog) , MP_ROM_INT(ANALOG)},
    { MP_ROM_QSTR(MP_QSTR_I2C) , MP_ROM_INT(I2C)},
    { MP_ROM_QSTR(MP_QSTR_TOF) , MP_ROM_INT(TOF)},
    { MP_ROM_QSTR(MP_QSTR_Touch) , MP_ROM_INT(TOUCH)},
    { MP_ROM_QSTR(MP_QSTR_UART) , MP_ROM_INT(UART)},
};
static MP_DEFINE_CONST_DICT(sensor_local, sensor_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sensor_type,
    MP_QSTR_sensor,
    MP_TYPE_FLAG_NONE,
    make_new, mp_sensor_make_new,
    print, mp_sensor_print,
    locals_dict, &sensor_local
    );

