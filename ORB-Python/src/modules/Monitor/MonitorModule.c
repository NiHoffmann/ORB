#include "py/runtime.h"
#include "KeyConstants.h"

static mp_obj_t mp_get_key() {
    return mp_obj_new_int(getMonitorKey());
}
static MP_DEFINE_CONST_FUN_OBJ_0(mp_get_key_obj, mp_get_key);

static mp_obj_t mp_set_text(mp_obj_t arg) {
    size_t len;
    const char *str = mp_obj_str_get_data(arg, &len);
    setMonitorText(str, len);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(mp_set_text_obj, mp_set_text);

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
