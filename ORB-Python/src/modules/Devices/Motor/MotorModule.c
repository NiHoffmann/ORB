#include "py/obj.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "MotorModule.h"
#include "Motor_C_Interface.h"
#include "helper.h"
#include <stdio.h>

const mp_obj_type_t motor_type;

typedef struct _motor_obj_t {
    mp_obj_base_t base;
    uint8_t port;
    int8_t direction;
    uint16_t ticks;
    uint8_t acc;
    uint8_t kp;
    uint8_t ki;
    uint8_t mode;
    uint8_t speed;
    uint8_t position;
} motor_obj_t;

motor_obj_t motor_obj_list[4] = {
    { .base = { .type = &motor_type }, .port = 0, .direction = 0, .ticks = 0, .acc = 0, .kp = 0, .ki = 0, .mode = 0, .speed = 0, .position = 0 },
    { .base = { .type = &motor_type }, .port = 1, .direction = 0, .ticks = 0, .acc = 0, .kp = 0, .ki = 0, .mode = 0, .speed = 0, .position = 0 },
    { .base = { .type = &motor_type }, .port = 2, .direction = 0, .ticks = 0, .acc = 0, .kp = 0, .ki = 0, .mode = 0, .speed = 0, .position = 0 },
    { .base = { .type = &motor_type }, .port = 3, .direction = 0, .ticks = 0, .acc = 0, .kp = 0, .ki = 0, .mode = 0, .speed = 0, .position = 0 }
};

static void mp_motor_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    motor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "Motor(%d)", self->port);
}

static mp_obj_t mp_motor_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_port, ARG_direction, ARG_ticks, ARG_acc, ARG_kp, ARG_ki};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_port,         MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1 } },
        { MP_QSTR_direction,    MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
        { MP_QSTR_ticks,        MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 72 } },
        { MP_QSTR_acc,          MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 25 } },
        { MP_QSTR_kp,           MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 50 } },
        { MP_QSTR_ki,           MP_ARG_KW_ONLY  | MP_ARG_INT, {.u_int = 30 } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int port         =   args[ARG_port].u_int;
    float direction =  (args[ARG_direction].u_obj != MP_OBJ_NULL)?mp_obj_get_float(args[ARG_direction].u_obj):1;

    CHECK_VALID_PORT(port, motor_obj_list)

    motor_obj_t *self = &motor_obj_list[port];

    self->direction = (direction>=0)?FORWARD:REVERSE;
    self->ticks   =   args[ARG_ticks].u_int;
    self->acc     =   args[ARG_acc].u_int;
    self->kp      =   args[ARG_kp].u_int;
    self->ki      =   args[ARG_ki].u_int;

    motorSettings(self->port, self->ticks, self->acc, self->kp, self->ki );
    setMotor(self->port, self->mode, self->speed * self->direction , self->position * self->direction);

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t config(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args){
    enum { ARG_direction, ARG_ticks, ARG_acc, ARG_kp, ARG_ki};

    // Define the allowed arguments
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_direction,    MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
        { MP_QSTR_ticks,        MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL } },
        { MP_QSTR_acc,          MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
        { MP_QSTR_kp,           MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
        { MP_QSTR_ki,           MP_ARG_KW_ONLY  | MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
    };

    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1  , pos_args + 1 , kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    motor_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);

    ACCEPT_VALUE_KW_GIVEN(ARG_direction, self->direction, mp_obj_get_float);
    ACCEPT_VALUE_KW_GIVEN(ARG_ticks, self->ticks, mp_obj_get_int);
    ACCEPT_VALUE_KW_GIVEN(ARG_acc, self->acc , mp_obj_get_int);
    ACCEPT_VALUE_KW_GIVEN(ARG_kp, self->kp , mp_obj_get_int);
    ACCEPT_VALUE_KW_GIVEN(ARG_ki, self->ki , mp_obj_get_int);

    int port = self->port;
    motorSettings(port,self->ticks,self->acc,self->kp,self->ki);
    setMotor(self->port, self->mode, self->speed * self->direction , self->position * self->direction);

    return MP_OBJ_FROM_PTR(self);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(config_obj, 0, config);

static mp_obj_t set(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args){
    enum { ARG_mode, ARG_speed, ARG_position};

    // Define the allowed arguments
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mode,              MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
        { MP_QSTR_speed,             MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
        { MP_QSTR_position,          MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL  } },
    };

    // Parse the arguments
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];

    mp_arg_parse_all(n_args - 1  , pos_args + 1 , kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    motor_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);

    ACCEPT_VALUE_KW_GIVEN(ARG_mode, self->mode, mp_obj_get_int);
    ACCEPT_VALUE_KW_GIVEN(ARG_speed, self->speed, mp_obj_get_int);
    ACCEPT_VALUE_KW_GIVEN(ARG_position, self->position , mp_obj_get_int);

    setMotor(self->port, self->mode, self->speed * self->direction , self->position * self->direction);

    return MP_OBJ_FROM_PTR(self);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(set_obj, 1, set);

static mp_obj_t get(mp_obj_t self_in){
    motor_obj_t *self = MP_OBJ_TO_PTR(self_in);
    motor_return_values *ret = getMotor(self->port);

    mp_obj_t dict = mp_obj_new_dict(3);

    mp_obj_dict_store(dict, mp_obj_new_str("speed", 5), mp_obj_new_int(ret->speed));
    mp_obj_dict_store(dict, mp_obj_new_str("position", 8), mp_obj_new_int(ret->pos));
    mp_obj_dict_store(dict, mp_obj_new_str("power", 5), mp_obj_new_int(ret->pwr));

    return dict;
}
static MP_DEFINE_CONST_FUN_OBJ_1(get_obj , get);

// ADDITIONAL FUNCTIONS
    static mp_obj_t motor_brake(mp_obj_t self_in) {
        motor_obj_t *self = MP_OBJ_TO_PTR(self_in);

        brakeMotor(self->port);

        return MP_OBJ_FROM_PTR(self);
    }
    static MP_DEFINE_CONST_FUN_OBJ_1(motor_brake_obj, motor_brake);

    static mp_obj_t get_motor_speed(mp_obj_t self_in) {
        motor_obj_t *self = MP_OBJ_TO_PTR(self_in);
        return mp_obj_new_int(getMotorSpeed(self->port));
    }
    static MP_DEFINE_CONST_FUN_OBJ_1(get_motor_speed_obj, get_motor_speed);

    static mp_obj_t get_motor_power(mp_obj_t self_in) {
        motor_obj_t *self = MP_OBJ_TO_PTR(self_in);
        return mp_obj_new_int(getMotorPower(self->port));
    }
    static MP_DEFINE_CONST_FUN_OBJ_1(get_motor_power_obj, get_motor_power);

    static mp_obj_t get_motor_position(mp_obj_t self_in) {
        motor_obj_t *self = MP_OBJ_TO_PTR(self_in);
        return mp_obj_new_int(getMotorPosition(self->port));
    }
    static MP_DEFINE_CONST_FUN_OBJ_1(get_motor_position_obj, get_motor_position);

// END ADDITIONAL FUNCTIONS

static const mp_rom_map_elem_t motor_local_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_M1), MP_ROM_INT(M1) },
    { MP_ROM_QSTR(MP_QSTR_M2), MP_ROM_INT(M2) },
    { MP_ROM_QSTR(MP_QSTR_M3), MP_ROM_INT(M3) },
    { MP_ROM_QSTR(MP_QSTR_M4), MP_ROM_INT(M4) },
    { MP_ROM_QSTR(MP_QSTR_FORWARD), MP_ROM_INT(FORWARD) },
    { MP_ROM_QSTR(MP_QSTR_REVERSE), MP_ROM_INT(REVERSE) },

    { MP_ROM_QSTR(MP_QSTR_POWER_MODE), MP_ROM_INT(POWER_MODE) },
    { MP_ROM_QSTR(MP_QSTR_BRAKE_MODE), MP_ROM_INT(BRAKE_MODE) },
    { MP_ROM_QSTR(MP_QSTR_SPEED_MODE), MP_ROM_INT(SPEED_MODE) },
    { MP_ROM_QSTR(MP_QSTR_MOVETO_MODE), MP_ROM_INT(MOVETO_MODE) },

    { MP_ROM_QSTR(MP_QSTR_config), MP_ROM_PTR(&config_obj) },
    { MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&set_obj) },
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&get_obj) },

    { MP_ROM_QSTR(MP_QSTR_brake), MP_ROM_PTR(&motor_brake_obj) },
    { MP_ROM_QSTR(MP_QSTR_getSpeed), MP_ROM_PTR(&get_motor_speed_obj) },
    { MP_ROM_QSTR(MP_QSTR_getPower), MP_ROM_PTR(&get_motor_power_obj) },
    { MP_ROM_QSTR(MP_QSTR_getPosition), MP_ROM_PTR(&get_motor_position_obj) },
};
static MP_DEFINE_CONST_DICT(motor_local, motor_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    motor_type,
    MP_QSTR_motor,
    MP_TYPE_FLAG_NONE,
    make_new, mp_motor_make_new,
    print, mp_motor_print,
    locals_dict, &motor_local
    );

