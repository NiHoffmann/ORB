
#define HEAP_SIZE_KB                            (32)

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
#define ORB_EXIT_GC_ABORT                       (3)

#define ORB_ENABLED_AUTOMATIC_GC                (1)
//even at 100% this will assume ~500byte are reserved for the vm's internal operations
#define ORB_AUTOMATIC_GC_THRESHOLD_IN_PERCENT   (90)
#define ORB_AUTOMATIC_GC_VM_RESERVED_BYTE       (512)

#define ORB_ABORT_ON_GC_COLLECT_FAIL            (1)

#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define OMIT_FRAME_POINTER_ON_WINDOWS           (1)
