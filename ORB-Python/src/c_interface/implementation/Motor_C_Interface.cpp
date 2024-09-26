#include "stdint.h"
#include "AppTask.h"
#include "lib.h"

extern "C" {

    void motorSettings(uint8_t port, uint32_t ticsPerRotation,uint8_t acc,uint8_t kp,uint8_t ki ){
        AppTask::configMotor(nullptr ,port,  ticsPerRotation, acc, kp, ki);
    }

    void setMotor( uint8_t  port, uint8_t  mode, int16_t speed, int pos ){
        AppTask::setMotor(nullptr, port , mode , speed, pos);
    }

    typedef struct _motor_return_values{
        float pwr;
        float speed;
        int pos;
    }motor_return_values;

    motor_return_values* getMotor(uint8_t port) {
        ORB::Motor m =  AppTask::getMotor(nullptr, port);

        motor_return_values* values = (motor_return_values*)malloc(sizeof(motor_return_values));
        if (values == NULL) {
            return NULL;
        }


        values->pwr =  m.pwr;
        values->speed = m.speed;
        values->pos = m.pos;

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
}
