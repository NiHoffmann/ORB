//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#include "python-vm.h"
#include <cstdlib>
#include <stdio.h>

extern "C"{
    #include "python-vm.h"
    #include "py/obj.h"
    #include "py/mphal.h"
    #include "py/runtime.h"
    #include "port/micropython_embed.h"
    #include "mpconfigport.h"
}

bool PythonVM::running = false;

PythonVM::PythonVM() {}

PythonVM::~PythonVM() {}

static char heap[HEAP_SIZE_KB * 1024] = {0};

void PythonVM::initVM() {
    int stack_top = 0;

    #if ORB_ENABLE_EXIT_STATUS
    MP_STATE_VM(orb_exit_status) = ORB_EXIT_NORMAL;
    if (MP_STATE_VM(orb_exit_message)) {
        free(MP_STATE_VM(orb_exit_message));
    }
    MP_STATE_VM(orb_exit_message) = NULL;
    #endif

    #if ORB_ENABLE_INTERRUPT
    MP_STATE_VM(orb_interrupt) = false;
    MP_STATE_VM(orb_interrupt_injected) = false;
    #endif

    #ifdef ORB_ABORT_ON_GC_COLLECT_FAIL
    MP_STATE_VM(orb_gc_abort) = false;
    #endif

    mp_embed_init(&heap[0], sizeof(heap), &stack_top);
}

#define MODULE_LENGTH_BYTES (4)
#define NUM_MODULES_BYTE    (1)
void PythonVM::startProgram(LoadLengthFunction loadLength, LoadProgramFunction loadProgram) {
    int length = loadLength();
    //load program should now be called load frame or something....
    uint8_t* byteArray = loadProgram(length);
    uint8_t num_modules = byteArray[0];
    uint32_t module_length = 0;
    //we start after the last module length value
    uint32_t current_position = NUM_MODULES_BYTE + (MODULE_LENGTH_BYTES * num_modules);

    for(int i=0;i<num_modules;i++){
        module_length = (byteArray[(i*MODULE_LENGTH_BYTES) + 1] << 24) | (byteArray[(i*MODULE_LENGTH_BYTES) + 2] << 16) | (byteArray[(i*MODULE_LENGTH_BYTES) + 3] << 8) | byteArray[(i*MODULE_LENGTH_BYTES) + 4];
        mp_embed_exec_mpy(&byteArray[current_position], module_length);
        current_position += module_length;
    }
    free(byteArray);
}

void PythonVM::deinitVM() {
    MP_STATE_VM(devices_initialised) = false;
    mp_embed_deinit();
}

void PythonVM::run(LoadLengthFunction loadLength, LoadProgramFunction loadProgram, uint8_t arg) {
    running = true;
    MP_STATE_VM(arg) = arg;
    initVM();
    startProgram(loadLength, loadProgram);
    deinitVM();
    running = false;
}

bool PythonVM::isRunning() {
    return running;
}

void PythonVM::stopProgram() {
    #if ORB_ENABLE_INTERRUPT
    MP_STATE_VM(orb_interrupt) = true;
    #endif
}

int PythonVM::getExitStatus(){
    #if ORB_ENABLE_EXIT_STATUS
    return MP_STATE_VM(orb_exit_status);
    #endif
}

const char* PythonVM::getExitInfo(){
    #if ORB_ENABLE_EXIT_STATUS
    if(MP_STATE_VM(orb_exit_status) == INTERRUPT)
        return "User Interrupt";
    #if ORB_ABORT_ON_GC_COLLECT_FAIL
        if(MP_STATE_VM(orb_exit_status) == ORB_EXIT_GC_ABORT)
            return "GC Abort";
    #endif
    //i cant use NULL in EmBitz
    if( MP_STATE_VM(orb_exit_message) == 0 &&  MP_STATE_VM(orb_exit_status) == NORMAL){
        return "VM Normal Exit";
    }

    return MP_STATE_VM(orb_exit_message);
    #endif
    return nullptr;
}
