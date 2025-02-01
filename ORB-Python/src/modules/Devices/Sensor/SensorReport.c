
//*******************************************************************
/*!
\author Nils Hoffmann
\date   01.02.2025
*/

#include "py/obj.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stackctrl.h"
#include "SensorReport.h"
#include "helper.h"

typedef struct _sensor_report_obj_t {
    mp_obj_base_t base;
    uint32_t value1;
    uint32_t value2;
    uint8_t  type;
    uint8_t  option;
    uint8_t  lenExp;
} sensor_report_obj_t;

static void sensor_report_struct_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    sensor_report_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (dest[0] == MP_OBJ_NULL) {
        switch(attr){
            case MP_QSTR_values:
                {
                    mp_obj_t values = mp_obj_new_list(0, NULL);
                    mp_obj_list_append(values, mp_obj_new_int(self->value1));
                    mp_obj_list_append(values, mp_obj_new_int(self->value2));
                    dest[0] = values;
                }
                break;
            case MP_QSTR_type:
                dest[0] = mp_obj_new_int(self->type);
                break;
            case MP_QSTR_option:
                dest[0] = mp_obj_new_int(self->option);
                break;
            case MP_QSTR_lenExp:
                dest[0] = mp_obj_new_int(self->lenExp);
                break;
        }
    } else {
        mp_raise_ValueError(MP_ERROR_TEXT("A Sensor Report is Read-Only."));
    }
}

static void mp_sensor_report_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    //sensor_report_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<SensorReport>");
}

mp_obj_t new_sensor_report(uint32_t *value, uint8_t  type, uint8_t  option, uint8_t  lenExp) {
    sensor_report_obj_t *self = m_new_obj(sensor_report_obj_t);
    self->base.type = &sensor_report_type;

    self->value1 = value[0];
    self->value2 = value[1];

    self->type = type;
    self->option = option;
    self->lenExp = lenExp;

    return MP_OBJ_FROM_PTR(self);
}

static const mp_rom_map_elem_t sensor_report_local_dict_table[] = {};
MP_DEFINE_CONST_DICT(sensor_report_local, sensor_report_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sensor_report_type,
    MP_QSTR_SensorReport,
    MP_TYPE_FLAG_NONE,
    print, mp_sensor_report_print,
    locals_dict, &sensor_report_local,
    attr, sensor_report_struct_attr
    );

