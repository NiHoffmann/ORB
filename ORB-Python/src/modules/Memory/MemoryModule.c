// Include MicroPython API.
#include "py/runtime.h"
#include "memory.h"

static mp_obj_t memory___init__(void) {
    if (!MP_STATE_VM(memory_initialised)) {
        MP_STATE_VM(memory_initialised) = true;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(memory___init___obj, memory___init__);
MP_REGISTER_ROOT_POINTER(int memory_initialised);

static mp_obj_t mp_clear_memory() {
    clearMemory();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(mp_clear_memory_obj, mp_clear_memory);

static mp_obj_t mp_get_memory(mp_obj_t addr_obj, mp_obj_t length_obj) {
    uint32_t addr = mp_obj_get_int(addr_obj);
    uint32_t length = mp_obj_get_int(length_obj);

    uint8_t* memory_pointer = getMemory(addr, length);
    mp_obj_t bytes_obj  = mp_obj_new_bytes(memory_pointer, length);
    free(memory_pointer);

    return bytes_obj;
}
static MP_DEFINE_CONST_FUN_OBJ_2(mp_get_memory_obj, mp_get_memory);

static mp_obj_t mp_set_memory(mp_obj_t addr_obj, mp_obj_t data_obj) {
    if (!mp_obj_is_type(data_obj, &mp_type_list)) {
        mp_raise_TypeError("Data should be passed as list");
    }

    uint32_t addr = mp_obj_get_int(addr_obj);
    mp_obj_list_t *list = MP_OBJ_TO_PTR(data_obj);
    uint32_t length = list->len;
    uint8_t *array = (uint8_t *)malloc(length * sizeof(uint8_t));

    for(int i =0; i < length;i++){
        array[i] = mp_obj_get_int(list->items[i]);
    }

    setMemory(addr, array, length);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(mp_set_memory_obj, mp_set_memory);

static const mp_rom_map_elem_t memory_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_memory) },
    { MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&memory___init___obj) },

    { MP_ROM_QSTR(MP_QSTR_clearMemory), MP_ROM_PTR(&mp_clear_memory_obj) },
    { MP_ROM_QSTR(MP_QSTR_setMemory), MP_ROM_PTR(&mp_set_memory_obj) },
    { MP_ROM_QSTR(MP_QSTR_getMemory), MP_ROM_PTR(&mp_get_memory_obj) },
};
static MP_DEFINE_CONST_DICT(memory_globals, memory_globals_table);

const mp_obj_module_t memory_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&memory_globals,
};

MP_REGISTER_MODULE(MP_QSTR_memory, memory_module);
