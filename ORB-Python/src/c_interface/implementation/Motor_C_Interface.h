#ifndef MOTOR_MOCKUP_H
#define MOTOR_MOCKUP_H
#include "stdint.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif


enum Port
{
  M1  = 0x00,
  M2,
  M3,
  M4
};

enum Direction
{
    FORWARD = +1,
    REVERSE = -1,
};

enum MotorMode
{
  POWER_MODE  = 0,
  BRAKE_MODE  = 1,
  SPEED_MODE  = 2,
  MOVETO_MODE = 3,
};

void motorSettings(uint8_t port, uint32_t ticsPerRotation,uint8_t acc, uint8_t kp, uint8_t ki );

void setMotor(uint8_t port, uint8_t mode, int16_t speed, int pos );

typedef struct _motor_return_values{
	int16_t pwr;
	int16_t speed;
	int pos;
}motor_return_values;

motor_return_values* getMotor(uint8_t port);

//ADDITIONAL FUNCTIONS

    void setMotorPower(uint8_t port, int8_t direction, short power);

    void setMotorSpeed(uint8_t port, int8_t direction, short speed);

    void setMotorPosition(uint8_t port, int8_t direction, short speed, int position);

    void brakeMotor(uint8_t port);

    int getMotorPower(uint8_t port);

    int getMotorSpeed(uint8_t port);

    int getMotorPosition(uint8_t port);

//END ADDITIONAL FUNCTIONS

#ifdef __cplusplus
}
#endif


#endif // MOTOR_H
