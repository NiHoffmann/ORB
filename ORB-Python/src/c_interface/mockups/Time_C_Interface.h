#ifndef TIME_MOCKUP_H
#define TIME_MOCKUP_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
#include "stdio.h"

uint32_t getTime(){
    printf("getTime()\n");
    return 0;
}

void wait(uint32_t time){
    printf("wait(%d)", time);
}

#ifdef __cplusplus
} 
#endif


#endif // TIME_MOCKUP_H
