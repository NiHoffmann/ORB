//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

// Include MicroPython API.
#include "py/runtime.h"
#include "Time_C_Interface.h"

extern const mp_obj_module_t time_module;

static mp_obj_t time___init__(void) {
    if (!MP_STATE_VM(time_initialised)) {
        MP_STATE_VM(time_initialised) = true;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(time___init___obj, time___init__);
MP_REGISTER_ROOT_POINTER(int time_initialised);

//TODO maybe dont keep this as time functions
static mp_obj_t mp_get_time() {
    return mp_obj_new_int(getTime());
}
static MP_DEFINE_CONST_FUN_OBJ_0(mp_get_time_obj, mp_get_time);

static mp_obj_t mp_wait(mp_obj_t arg) {
    wait(mp_obj_get_int(arg));
    return MP_OBJ_FROM_PTR(&time_module);
}
static MP_DEFINE_CONST_FUN_OBJ_1(mp_wait_obj, mp_wait);

static const mp_rom_map_elem_t time_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_time) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&time___init___obj) },

    { MP_ROM_QSTR(MP_QSTR_getTime), MP_ROM_PTR(&mp_get_time_obj) },
    { MP_ROM_QSTR(MP_QSTR_wait), MP_ROM_PTR(&mp_wait_obj) },

};
static MP_DEFINE_CONST_DICT(time_globals, time_globals_table);

const mp_obj_module_t time_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&time_globals,
};

MP_REGISTER_MODULE(MP_QSTR_time, time_module);
