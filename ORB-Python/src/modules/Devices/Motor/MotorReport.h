//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/
#ifndef MOTOR_REPORT_H
#define MOTOR_REPORT_H

#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"

extern const mp_obj_type_t motor_report_type;

extern mp_obj_t new_motor_report(int speed, int position, int power);

#endif // MOTOR_REPORT_H
