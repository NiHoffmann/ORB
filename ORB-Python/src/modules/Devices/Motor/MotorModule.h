#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stackctrl.h"

#define SET_MOTOR(self) \
    setMotor((self)->port, \
             (self)->mode, \
             CONDITIONAL_CLAMP((self)->mode == SPEED_MODE, \
                               (self)->speed * (self)->direction, \
                               -100, 100), \
             (self)->position * (self)->direction)

#define UPDATE_MOTOR_SETTINGS(self) \
    SET_MOTOR(self);\
    motorSettings((self)->port, (self)->ticks, (self)->acc, (self)->kp, (self)->ki)

extern const mp_obj_type_t motor_type;

void init_motor_representations();
