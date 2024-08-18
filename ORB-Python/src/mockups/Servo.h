#ifndef SERVO_MOCKUP_H
#define SERVO_MOCKUP_H
#include <stdio.h>
enum ServoPort_type{
  S1 = 0,
  S2 = 1,
};

void setModelServo(uint16_t port, uint16_t speed, uint16_t angle){
    printf("set servo(%d) speed(%d) angle(%d)\n", port, speed, angle);
}
#endif // SERVO_MOCKUP_H
