//*******************************************************************
/*!
\author Nils Hoffmann
\date   01.02.2025
*/
#ifndef SENSOR_REPORT_H
#define SENSOR_REPORT_H

#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"

extern const mp_obj_type_t sensor_report_type;

extern mp_obj_t new_sensor_report(uint32_t *value, uint8_t  type, uint8_t  option, uint8_t  lenExp);

#endif // SENSOR_REPORT_H
