#ifndef PYTHON_VM_H
#define PYTHON_VM_H

#include <cstdint>
#include "mpconfigport.h"

// Function pointer types
typedef uint8_t* LoadProgramFunction(int length);
typedef uint32_t LoadLengthFunction();

// PythonVM class declaration
class PythonVM {
public:
    PythonVM();
    ~PythonVM();

    void run(LoadLengthFunction loadLength, LoadProgramFunction loadProgram);
    bool isRunning();
    void stopProgram();

    #ifdef ORB_ENABLE_EXIT_STATUS
    enum Status{
        NORMAL = ORB_EXIT_NORMAL,
        EXCEPTION = ORB_EXIT_EXCEPTION,
        INTERRUPT = ORB_EXIT_INTERRUPT,
        #ifdef ORB_ABORT_ON_GC_COLLECT_FAIL
        GC_ABORT = ORB_EXIT_GC_ABORT,
        #endif
    };
    #endif

    int getExitStatus();
    const char* getExitType();

private:
    static void initVM();
    static void startProgram(LoadLengthFunction loadLength, LoadProgramFunction loadProgram);
    static void deinitVM();

    static bool running;
};

#endif // PYTHON_VM_H
