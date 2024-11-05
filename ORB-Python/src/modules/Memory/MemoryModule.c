//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

// Include MicroPython API.
#include "py/runtime.h"
#include "Memory_C_Interface.h"

extern const mp_obj_module_t memory_module;

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
    return MP_OBJ_FROM_PTR(&memory_module);
}
static MP_DEFINE_CONST_FUN_OBJ_0(mp_clear_memory_obj, mp_clear_memory);

static mp_obj_t mp_get_memory(mp_obj_t addr_obj, mp_obj_t length_obj) {
    uint32_t addr = mp_obj_get_int(addr_obj);
    uint32_t length = mp_obj_get_int(length_obj);

    //TODO make this function take care of malloc not the interface method
    uint8_t* memory_pointer = getMemory(addr, length);
    mp_obj_t bytes_obj  = mp_obj_new_bytes(memory_pointer, length);
    //we can free the memory since mp_obj_new_bytes copies the data
    free(memory_pointer);

    return bytes_obj;
}
static MP_DEFINE_CONST_FUN_OBJ_2(mp_get_memory_obj, mp_get_memory);

static mp_obj_t mp_set_memory(mp_obj_t addr_obj, mp_obj_t data_obj) {
    if (!mp_obj_is_type(data_obj, &mp_type_list) &&
        !mp_obj_is_type(data_obj, &mp_type_bytes)) {
        mp_raise_TypeError("Data should be passed as list, bytes, or bytearray");
    }

    uint32_t addr = mp_obj_get_int(addr_obj);
    uint8_t *array;
    uint32_t length;

    if (mp_obj_is_type(data_obj, &mp_type_list)) {
        mp_obj_list_t *list = MP_OBJ_TO_PTR(data_obj);
        length = list->len;
        array = (uint8_t *)malloc(length * sizeof(uint8_t));
        for (int i = 0; i < length; i++) {
            array[i] = mp_obj_get_int(list->items[i]);
        }
    } else if (mp_obj_is_type(data_obj, &mp_type_bytes)) {
        mp_buffer_info_t bufinfo;
        mp_get_buffer_raise(data_obj, &bufinfo, MP_BUFFER_READ);
        array = (uint8_t *)malloc(bufinfo.len * sizeof(uint8_t));
        memcpy(array, bufinfo.buf, bufinfo.len);
        length = bufinfo.len;
    }

    setMemory(addr, array, length);
    free(array);

    return MP_OBJ_FROM_PTR(&memory_module);
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
