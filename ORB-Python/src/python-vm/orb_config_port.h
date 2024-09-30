/*
The original C standard (ANSI 1989/ISO 1990) required that a compiler successfully translate at least one program containing at least one example of a set of environmental limits. One of those limits was being able to create an object of at least 32,767 bytes.
This minimum limit was raised in the 1999 update to the C standard to be at least 65,535 bytes.
No C implementation is required to provide for objects greater than that size, which means that they don't need to allow for an array of ints greater than (int)(65535 / sizeof(int)).
*/
#define HEAP_SIZE_KB                            (16)
#define STACK_SIZE_KB                           (16)
//Memory-Layout: <FLAG><PROGRAM_LENGTH><PROGRAM_BYTE_CODE>
#define PROGRAM_LENGTH_BYTE                     (4)
#define LANGUAGE_FLAG_BYTE                      (1)

// Use the minimal starting configuration (disables all optional features).
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)

#define MICROPY_PERSISTENT_CODE_LOAD            (1)

#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_PY_GC                           (1)
#define ORB_ENABLE_INTERRUPT					(1)
#define ORB_ENABLE_MONITOR_STD_OUT				(1)

#define ORB_ENABLE_EXIT_STATUS                  (1)
#define ORB_EXIT_NORMAL                         (0)
#define ORB_EXIT_EXCEPTION                      (1)
#define ORB_EXIT_INTERRUPT                      (2)

//this was a terrible idea, we only lose performance here
/*
#define ORB_EXIT_GC_ABORT                       (3)
#define ORB_ENABLED_AUTOMATIC_GC                (1)
//even at 100% this will assume ~500byte are reserved for the vm's internal operations
#define ORB_AUTOMATIC_GC_THRESHOLD_IN_PERCENT   (90)
#define ORB_AUTOMATIC_GC_VM_RESERVED_BYTE       (512)

#define ORB_ABORT_ON_GC_COLLECT_FAIL            (1)
*/

#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define OMIT_FRAME_POINTER_ON_WINDOWS           (1)
