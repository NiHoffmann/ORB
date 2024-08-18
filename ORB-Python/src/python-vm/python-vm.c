#include "python-vm.h"

bool running = false;
bool starting = false;

void initVM(){
    int stack_top = 0;
    char heap[32 * 1024] = {0};

    MP_STATE_VM(orb_interrupt) = false;
    MP_STATE_VM(orb_interrupt_injected) = false;
    mp_embed_init(&heap[0], sizeof(heap), &stack_top);
}

void startProgram(loadProgramFunction loadProgram){
    uint8_t* byteArray = loadProgram();
    uint32_t length = (byteArray[0] << 24) | (byteArray[1] << 16) | (byteArray[2] << 8) | byteArray[3];
    mp_embed_exec_mpy(&byteArray[4], length);
}

void deinitVM(){
    mp_embed_deinit();
}

//Interface
void run(loadProgramFunction loadProgram){

    running = true;
    initVM();
    startProgram(loadProgram);
    deinitVM();
    running = false;
}

bool isRunning(){
    return running;
}

//This will result in a memory access violation
void forceStopProgram(){
    MP_STATE_VM(orb_interrupt) = true;
}
