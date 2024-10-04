#ifndef MOTOR_MOCKUP_H
#define MOTOR_MOCKUP_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

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

void motorSettings(uint8_t port, uint16_t ticsPerRotation,uint8_t acc,uint8_t kp,uint8_t ki ){
    printf("motor settings\nport(%u) ticks(%u) acc(%u) kp(%u) ki(%u)\n",port,  ticsPerRotation, acc, kp, ki);
}

void setMotor( uint8_t  port, uint8_t  mode, int16_t speed, int pos ){
    printf("set motor port(%d) mode(%d) speed(%d) pos(%d)\n", port , mode , speed, pos);
}

typedef struct _motor_return_values{
	float pwr;
	float speed;
	int pos;
}motor_return_values;

motor_return_values* getMotor(uint8_t port) {
    motor_return_values* values = (motor_return_values*)malloc(sizeof(motor_return_values));
    if (values == NULL) {
        return NULL;
    }

    values->pwr = 1.0f;
    values->speed = 2.0f;
    values->pos = 3;

    return values;
}

//ADDITIONAL FUNCTIONS

    void setMotorPower(uint8_t port, int8_t direction, short power){
        printf("motor power\n");
    }

    void setMotorSpeed(uint8_t port, int8_t direction, short speed){
        printf("motor speed\n");
    }

    void setMotorPosition(uint8_t port, int8_t direction, short speed, int position){
        printf("motor position\n");
    }

    void brakeMotor(uint8_t port){
        printf("brake motor\n");
    }

    int getMotorPower(uint8_t port){
        return 1;
    }

    int getMotorSpeed(uint8_t port){
        return 2;
    }

    int getMotorPosition(uint8_t port){
        return 3;
    }

//END ADDITIONAL FUNCTIONS

#ifdef __cplusplus
}
#endif


#endif // MOTOR_H
