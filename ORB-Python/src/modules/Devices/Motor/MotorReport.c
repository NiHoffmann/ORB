
//*******************************************************************
/*!
\author Nils Hoffmann
\date   30.01.2025
*/

#include "py/obj.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stackctrl.h"
#include "MotorReport.h"
#include "helper.h"

typedef struct _motor_report_obj_t {
    mp_obj_base_t base;
    int speed;
    int position;
    int power;
} motor_report_obj_t;

static void motor_report_struct_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    motor_report_obj_t *self = MP_OBJ_TO_PTR(self_in);
    //dest[0] will be null if we have to "self" aka read operation
    if (dest[0] == MP_OBJ_NULL) {
        switch(attr){
            case MP_QSTR_speed:
                dest[0] = mp_obj_new_int(self->speed);
                break;
            case MP_QSTR_position:
                dest[0] = mp_obj_new_int(self->position);
                break;
            case MP_QSTR_power:
                dest[0] = mp_obj_new_int(self->power);
                break;
        }
    } else {
        switch(attr){
            case MP_QSTR_speed:
               //dest[1] is the input value
               self->speed = MP_OBJ_GET_NUMBER_VALUE(dest[1]);
                //Indicate a succecful operation
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_position:
                self->position = MP_OBJ_GET_NUMBER_VALUE(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
            case MP_QSTR_power:
                self->power = MP_OBJ_GET_NUMBER_VALUE(dest[1]);
                dest[0] = MP_OBJ_NULL;
                break;
        }
    }
}

static void mp_motor_report_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    motor_report_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<MotorReport, power:%d , speed:%d , position:%d >", self->power, self->speed, self->position);
}

mp_obj_t new_motor_report(int speed, int position, int power) {
    motor_report_obj_t *self = m_new_obj(motor_report_obj_t);
    self->base.type = &motor_report_type;

    self->speed = speed;
    self->position = position;
    self->power = power;

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t motor_report_get_speed(mp_obj_t self_in) {
    motor_report_obj_t *self = MP_OBJ_TO_PTR(self_in);
    return MP_OBJ_NEW_SMALL_INT(self->speed);
}
MP_DEFINE_CONST_FUN_OBJ_0(motor_report_get_speed_obj, motor_report_get_speed);

static const mp_rom_map_elem_t motor_report_local_dict_table[] = {};
MP_DEFINE_CONST_DICT(motor_report_local, motor_report_local_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    motor_report_type,
    MP_QSTR_MotorReport,
    MP_TYPE_FLAG_NONE,
    print, mp_motor_report_print,
    locals_dict, &motor_report_local,
    attr, motor_report_struct_attr
    );

