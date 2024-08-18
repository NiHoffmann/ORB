#include "EV3touchType.h"
#include "Sensor.h"
#include "helper.h"

const mp_obj_type_t ev3_touch_type;

static void _conf(int port){
    configSensor(port, TOUCH, 0, 0);
}

static mp_obj_t _get(sensor_return_values  *ret){
    return mp_obj_new_int(ret->value[0]);
}

GENERATE_SIMPLE_SENSOR(ev3_touch, _get, _conf);
