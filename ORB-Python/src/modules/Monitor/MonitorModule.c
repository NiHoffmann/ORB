//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#include "py/runtime.h"
#include "KeyConstants.h"
#include "Monitor_C_Interface.h"

//forward declaration to access in method returns
extern const mp_obj_module_t monitor_module;

static mp_obj_t mp_get_key() {
    return mp_obj_new_int(getMonitorKey());
}
static MP_DEFINE_CONST_FUN_OBJ_0(mp_get_key_obj, mp_get_key);

static mp_obj_t mp_set_text(mp_obj_t line_obj, mp_obj_t text_obj) {
    size_t len;
    const int line = mp_obj_int_get_checked(line_obj);
    const char *str;

    if (mp_obj_is_str(text_obj)) {
        str = mp_obj_str_get_data(text_obj, &len);
    } else {
        mp_obj_t str_obj = mp_obj_str_make_new(&mp_type_str, 1, 0, &text_obj);
        str = mp_obj_str_get_data(str_obj, &len);
    }

    setMonitorText(line, str, len);

    return MP_OBJ_FROM_PTR(&monitor_module);
}
static MP_DEFINE_CONST_FUN_OBJ_2(mp_set_text_obj, mp_set_text);

static const mp_rom_map_elem_t monitor_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_monitor) },

    { MP_ROM_QSTR(MP_QSTR_getKey), MP_ROM_PTR(&mp_get_key_obj) },
    { MP_ROM_QSTR(MP_QSTR_setText), MP_ROM_PTR(&mp_set_text_obj) },
    { MP_ROM_QSTR(MP_QSTR_keys), MP_ROM_PTR(&key_constants) },
};
static MP_DEFINE_CONST_DICT(monitor_module_globals, monitor_module_globals_table);

const mp_obj_module_t monitor_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&monitor_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_monitor, monitor_module);
