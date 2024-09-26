#include <stdio.h>
#include "AppTask.h"

extern "C" {

    void clearMemory(){
        AppTask::clearMemory(nullptr);
    }

    void setMemory(uint32_t addr, uint8_t *data, uint32_t data_size){
        AppTask::setMemory(nullptr, addr, data, data_size);
        free(data);
    }

    uint8_t* getMemory(uint32_t addr, uint32_t data_size){
        // Allocate memory for the byte array
        uint8_t* memory = (uint8_t*)malloc(data_size);

        AppTask::getMemory(nullptr, addr, memory, data_size);

        return memory;
    }

}
