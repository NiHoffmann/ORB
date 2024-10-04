#include "py/obj.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stackctrl.h"
#include "ExternButtonType.h"
#include "helper.h"
#include "Sensor_C_Interface.h"

const mp_obj_type_t extern_button_type;

typedef struct _extern_button_obj_t {
    mp_obj_base_t base;
    uint8_t id;
} extern_button_obj_t;

extern_button_obj_t extern_button_obj_list[2];

void init_extern_button_representation(){
    extern_button_obj_list[0] = (extern_button_obj_t) { .base = { .type = &extern_button_type }, .id = 0 };
    extern_button_obj_list[1] = (extern_button_obj_t) { .base = { .type = &extern_button_type }, .id = 1 };
}

static void mp_extern_button_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    const extern_button_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "extern_button(%d)", self->id);
}

static mp_obj_t mp_extern_button_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    enum { ARG_id};
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_id,         MP_ARG_REQUIRED | MP_ARG_INT, {.u_int = -1 } },
    };
    PARSE_KW_ARGS_CONSTRUCTOR(n_args, n_kw, all_args, allowed_args);

    int id = ACCEPT_ID(ARG_id, extern_button_obj_list);

    extern_button_obj_t *self = &extern_button_obj_list[id];

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t get_extern_button(mp_obj_t self_id) {
    extern_button_obj_t *self = MP_OBJ_TO_PTR(self_id);
    bool ret = getSensorDigital(self->id);
    return  mp_obj_new_bool(ret);
}
static MP_DEFINE_CONST_FUN_OBJ_1(get_extern_button_obj, get_extern_button);

static const mp_rom_map_elem_t extern_button_local_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&get_extern_button_obj) },
};
static MP_DEFINE_CONST_DICT(extern_button_local, extern_button_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    extern_button_type,
    MP_QSTR_extern_button,
    MP_TYPE_FLAG_NONE,
    make_new, mp_extern_button_make_new,
    print, mp_extern_button_print,
    locals_dict, &extern_button_local
    );
