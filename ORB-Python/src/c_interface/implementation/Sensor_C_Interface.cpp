#include "Sensor_C_Interface.h"
#include <stdio.h>
#include "AppTask.h"
#include "lib.h"

extern "C" {

    void configSensor (uint8_t id,uint8_t t,uint8_t m,uint32_t o) {
        AppTask::configSensor(nullptr, id ,t ,m ,o);
    }

    static sensor_return_values values = {.value={99,89},.type=1,.option=2,.lenExp=3};

    sensor_return_values* getSensor(uint8_t id){
        ORB::Sensor s =  AppTask::getSensor(nullptr, id);
        values.value[0] = s.value[0];
        values.value[1] = s.value[1];
        values.lenExp = s.lenExp;
        values.option = s.option;
        return &values;
    }

    uint32_t getSensorValueExt(uint8_t id, uint8_t ch ) {
        return AppTask::getSensorValueExt(nullptr, id , ch);
    }

    uint8_t getSensorDigital (uint8_t id) {
        return AppTask::getSensorDigital(nullptr, id);
    }

}
