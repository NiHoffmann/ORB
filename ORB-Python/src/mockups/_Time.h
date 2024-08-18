#ifndef TIME_MOCKUP_H
#define TIME_MOCKUP_H
#include "stdint.h"

uint32_t getTime(){
    return 99;
}

void wait(uint32_t time){
    printf("waiting ...");
}

#endif // TIME_MOCKUP_H
