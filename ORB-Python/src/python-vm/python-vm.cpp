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

void PythonVM::startProgram(LoadLengthFunction loadLength, LoadProgramFunction loadProgram) {
    int length = loadLength();
    uint8_t* byteArray = loadProgram(length);
    mp_embed_exec_mpy(byteArray, length);
    free(byteArray);
}

void PythonVM::deinitVM() {
    MP_STATE_VM(devices_initialised) = false;
    mp_embed_deinit();
}

void PythonVM::run(LoadLengthFunction loadLength, LoadProgramFunction loadProgram) {
    running = true;
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

#if ORB_ENABLE_EXIT_STATUS
static const char* _exit_message_user_intterupt = "User Interrupt";
#ifdef ORB_ABORT_ON_GC_COLLECT_FAIL
static const char* _exit_message_gc_abort = "GC Abort";
#endif
static const char* _exit_message_normal = "VM Normal Exit";
#endif

const char* PythonVM::getExitType(){

    #if ORB_ENABLE_EXIT_STATUS
    if(MP_STATE_VM(orb_exit_status) == INTERRUPT)
        return _exit_message_user_intterupt;
    #ifdef ORB_ABORT_ON_GC_COLLECT_FAIL
        if(MP_STATE_VM(orb_exit_status) == ORB_EXIT_GC_ABORT)
            return _exit_message_gc_abort;
    #endif
    //i cant use NULL in EmBitz
    if( MP_STATE_VM(orb_exit_message) == 0 &&  MP_STATE_VM(orb_exit_status) == NORMAL){
        return _exit_message_normal;
    }

    return MP_STATE_VM(orb_exit_message);
    #endif
    return nullptr;
}
