//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#include "py/runtime.h"
#include "KeyConstants.h"
#include "Monitor_C_Interface.h"

static const mp_rom_map_elem_t monitor_keys_module_locals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_monitor_keys) },
    { MP_ROM_QSTR(MP_QSTR_NO_KEY), MP_ROM_INT(Keine_Taste) },

    { MP_ROM_QSTR(MP_QSTR_A1), MP_ROM_INT(Taste_A1) },
    { MP_ROM_QSTR(MP_QSTR_A2), MP_ROM_INT(Taste_A2) },
    { MP_ROM_QSTR(MP_QSTR_A3), MP_ROM_INT(Taste_A3) },
    { MP_ROM_QSTR(MP_QSTR_A4), MP_ROM_INT(Taste_A4) },
    { MP_ROM_QSTR(MP_QSTR_A5), MP_ROM_INT(Taste_A5) },
    { MP_ROM_QSTR(MP_QSTR_A6), MP_ROM_INT(Taste_A6) },
    { MP_ROM_QSTR(MP_QSTR_A7), MP_ROM_INT(Taste_A7) },
    { MP_ROM_QSTR(MP_QSTR_A8), MP_ROM_INT(Taste_A8) },

    { MP_ROM_QSTR(MP_QSTR_B1), MP_ROM_INT(Taste_B1) },
    { MP_ROM_QSTR(MP_QSTR_B2), MP_ROM_INT(Taste_B2) },
    { MP_ROM_QSTR(MP_QSTR_B3), MP_ROM_INT(Taste_B3) },

    { MP_ROM_QSTR(MP_QSTR_B4), MP_ROM_INT(Taste_B4) },
    { MP_ROM_QSTR(MP_QSTR_B5), MP_ROM_INT(Taste_B5) },
    { MP_ROM_QSTR(MP_QSTR_B6), MP_ROM_INT(Taste_B6) },

    { MP_ROM_QSTR(MP_QSTR_B7), MP_ROM_INT(Taste_B7) },
    { MP_ROM_QSTR(MP_QSTR_B8), MP_ROM_INT(Taste_B8) },
    { MP_ROM_QSTR(MP_QSTR_B9), MP_ROM_INT(Taste_B9) },

    { MP_ROM_QSTR(MP_QSTR_B10), MP_ROM_INT(Taste_B10) },
    { MP_ROM_QSTR(MP_QSTR_B11), MP_ROM_INT(Taste_B11) },
    { MP_ROM_QSTR(MP_QSTR_B12), MP_ROM_INT(Taste_B12) },

    { MP_ROM_QSTR(MP_QSTR_C1), MP_ROM_INT(Taste_C1) },
};
static MP_DEFINE_CONST_DICT(monitor_keys_module_locals, monitor_keys_module_locals_table);

MP_DEFINE_CONST_OBJ_TYPE(
    key_constants,
    MP_QSTR_Key_Constants,
    MP_TYPE_FLAG_NONE,
    locals_dict, &monitor_keys_module_locals
    );
