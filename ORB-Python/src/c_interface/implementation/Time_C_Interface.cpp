#include "Time_C_Interface.h"
#include "stdint.h"
#include "AppTask.h"

extern "C" {
    uint32_t getTime(){
        return AppTask::getTime(nullptr);
    }

    void wait(uint32_t time){
        AppTask::wait(nullptr, time);
    }
}
