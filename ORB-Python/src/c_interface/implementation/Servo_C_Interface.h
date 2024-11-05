//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#ifndef SERVO_MOCKUP_H
#define SERVO_MOCKUP_H
#include "stdint.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

enum ServoPort_type{
  S1 = 0,
  S2 = 1,
};

void setModelServo(uint16_t port, uint16_t speed, uint16_t angle);

#ifdef __cplusplus
}
#endif
#endif // SERVO_MOCKUP_H
