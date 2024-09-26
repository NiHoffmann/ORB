#include "py/obj.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stackctrl.h"
#include "servoModule.h"
#include "Servo_C_Interface.h"
#include "helper.h"

const mp_obj_type_t servo_type;

typedef struct _servo_obj_t {
    mp_obj_base_t base;
    uint8_t port;
    uint8_t speed;
    uint8_t angle;
} servo_obj_t;

servo_obj_t servo_obj_list[2] = {
    { .base = { .type = &servo_type }, .port = 0, .speed = 0, .angle = 0 },
    { .base = { .type = &servo_type }, .port = 1, .speed = 0, .angle = 0 },
};

static void mp_servo_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    const servo_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "servo(%u)", self->port);
}

static mp_obj_t mp_servo_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_port};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_port,         MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1 } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    int port =   args[ARG_port].u_int;

    CHECK_VALID_PORT(port, servo_obj_list);

    servo_obj_t *self = &servo_obj_list[port];

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t set(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args){
    enum { ARG_speed, ARG_angle};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_speed, MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL } },
        { MP_QSTR_angle, MP_ARG_OBJ, {.u_obj  = MP_OBJ_NULL } },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1  , pos_args + 1 , kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    servo_obj_t *self = MP_OBJ_TO_PTR(pos_args[0]);

    if(kw_args->used > 0){
            ACCEPT_VALUE_KW_GIVEN(ARG_speed, self->speed, mp_obj_get_int);
            ACCEPT_VALUE_KW_GIVEN(ARG_angle, self->angle, mp_obj_get_int);
    } else {
        ACCEPT_VALUES_NO_KW_2(n_args - 1 , ARG_speed, self->speed, mp_obj_get_int, ARG_angle, self->angle, mp_obj_get_int);
    }

    setModelServo(self->port,self->speed,self->angle);

    return MP_OBJ_FROM_PTR(self);
}
static MP_DEFINE_CONST_FUN_OBJ_KW(set_obj, 0, set);

static const mp_rom_map_elem_t servo_local_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_S1), MP_ROM_INT(S1) },
    { MP_ROM_QSTR(MP_QSTR_S2), MP_ROM_INT(S2) },
    { MP_ROM_QSTR(MP_QSTR_set), MP_ROM_PTR(&set_obj) },
};
static MP_DEFINE_CONST_DICT(servo_local, servo_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    servo_type,
    MP_QSTR_servo,
    MP_TYPE_FLAG_NONE,
    make_new, mp_servo_make_new,
    print, mp_servo_print,
    locals_dict, &servo_local
    );

