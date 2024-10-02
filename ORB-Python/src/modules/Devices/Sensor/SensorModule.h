#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"

extern const mp_obj_type_t sensor_type;

typedef struct _sensor_obj_t {
    mp_obj_base_t base;
    uint8_t port;
    uint8_t type;
    uint8_t mode;
    uint32_t option;
} sensor_obj_t;

#endif
