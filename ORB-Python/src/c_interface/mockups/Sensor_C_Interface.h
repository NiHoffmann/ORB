//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#ifndef SENSOR_MOCKUP_H
#define SENSOR_MOCKUP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef enum
    {
      S1 = 0, //!< Sensorport 1
      S2,     //!< Sensorport 2
      S3,     //!< Sensorport 3
      S4      //!< Sensorport 4
    } SensorPort;

typedef enum
    {
      ANALOG = 0, //!< Analog-Sensor
      UART   = 1, //!< EV3-UART-Sensor
      I2C    = 2, //!< I2C-Sensor
      TOF    = 3, //!< Ultraschall-Sensor (Time Of Flight)
      TOUCH  = 4  //!< NXT/EV3-Touch-Sensor
    } SensorType;

void configSensor (uint8_t id,uint8_t t,uint8_t m,uint16_t o);

typedef struct _sensor_return_values{
	uint32_t value[2];
    uint8_t  type;
    uint8_t  option;
    uint8_t  lenExp;
}sensor_return_values;

sensor_return_values* getSensor(uint8_t id);

uint32_t getSensorValueExt(uint8_t id, uint8_t ch );

uint8_t getSensorDigital (uint8_t id);

#ifdef __cplusplus
}
#endif


#endif // SENSOR_MOCKUP_H
