#include "stdint.h"

#include "py/obj.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include <port/micropython_embed.h>

extern bool isInterrupted;

typedef uint8_t* loadProgramFunction();

extern void run(loadProgramFunction loadProgram);

extern bool isRunning();

extern void forceStopProgram();
