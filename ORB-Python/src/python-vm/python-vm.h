//*******************************************************************
/*!
\author Nils Hoffmann
\date   12.10.2024
*/

#ifndef PYTHON_VM_H
#define PYTHON_VM_H

#include <cstdint>
#include "mpconfigport.h"

typedef uint8_t* LoadProgramFunction(int length);
typedef uint32_t LoadLengthFunction();

class PythonVM {
public:
    PythonVM();
    ~PythonVM();   
    
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

    void run(LoadLengthFunction loadLength, LoadProgramFunction loadProgram, uint8_t arg);
    bool isRunning();
    void stopProgram();
    int getExitStatus();
    const char* getExitInfo();

private:
    static void initVM();
    static void startProgram(LoadLengthFunction loadLength, LoadProgramFunction loadProgram);
    static void deinitVM();

    static bool running;
};

#endif // PYTHON_VM_H
