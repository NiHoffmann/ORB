#include "Sensor.h"
#include <stdio.h>

void configSensor (uint8_t id,uint8_t t,uint8_t m,uint32_t o) {
    printf("Sensor id:%d type:%d mode:%d offset:%d\n",id ,t ,m ,o);
}

static sensor_return_values values = {.value={99,89},.type=1,.option=2,.lenExp=3};

sensor_return_values* getSensor(uint8_t id){
	return &values;
}

uint32_t getSensorValueExt(uint8_t id, uint8_t ch ) {
    return 77;
}

uint8_t getSensorDigital (uint8_t id) {
    return id == 0;
}
