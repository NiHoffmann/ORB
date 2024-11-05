//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#ifndef TIME_MOCKUP_H
#define TIME_MOCKUP_H
#include "stdint.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t getTime();

void wait(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif // TIME_MOCKUP_H
