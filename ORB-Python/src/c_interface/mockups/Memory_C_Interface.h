#ifndef MEMORY_MOCKUP_H
#define MEMORY_MOCKUP_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


void clearMemory(){
    printf("clearMemory()\n");
}

void setMemory(uint32_t addr, uint8_t *data, uint32_t data_size){
    printf("Writing to adrr(%d)\n", addr);
    for (uint8_t i = 0; i < data_size; i++) {
        printf("item: %d\n",data[i]);
    }
    free(data);
}

uint8_t* getMemory(uint32_t addr, uint32_t data_size){
    // Allocate memory for the byte array
    uint8_t* memory = (uint8_t*)malloc(data_size);

    for (uint8_t i = 0; i < data_size; i++) {
        memory[i] = i;
    }

    return memory;
}

#ifdef __cplusplus
}
#endif


#endif
