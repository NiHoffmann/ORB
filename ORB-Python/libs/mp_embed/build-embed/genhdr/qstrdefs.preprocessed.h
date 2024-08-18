# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "<stdin>"
# 29 "<stdin>"
# 1 "../../../micropython/py/mpconfig.h" 1
# 91 "../../../micropython/py/mpconfig.h"
# 1 "./mpconfigport.h" 1






# 1 "../../../micropython/ports/embed/port/mpconfigport_common.h" 1
# 27 "../../../micropython/ports/embed/port/mpconfigport_common.h"
# 1 "C:/Program Files/CodeBlocks/MinGW/lib/gcc/x86_64-w64-mingw32/8.1.0/include/stdint.h" 1 3 4
# 9 "C:/Program Files/CodeBlocks/MinGW/lib/gcc/x86_64-w64-mingw32/8.1.0/include/stdint.h" 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/stdint.h" 1 3 4
# 28 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/stdint.h" 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 1 3 4
# 10 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 1 3 4
# 12 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw_mac.h" 1 3 4
# 98 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4
             
# 107 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw_mac.h" 3 4
             
# 13 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw_secapi.h" 1 3 4
# 14 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 282 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/vadefs.h" 1 3 4
# 9 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/vadefs.h" 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 1 3 4
# 578 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/sdks/_mingw_directx.h" 1 3 4
# 579 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/sdks/_mingw_ddk.h" 1 3 4
# 580 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 10 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/vadefs.h" 2 3 4




#pragma pack(push,_CRT_PACKING)
# 24 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/vadefs.h" 3 4
  
# 24 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/vadefs.h" 3 4
 typedef __builtin_va_list __gnuc_va_list;






  typedef __gnuc_va_list va_list;
# 103 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/vadefs.h" 3 4
#pragma pack(pop)
# 283 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 2 3 4
# 552 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/_mingw.h" 3 4
void __attribute__((__cdecl__)) __debugbreak(void);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
{
  __asm__ __volatile__("int {$}3":);
}




const char *__mingw_get_crt_info (void);
# 11 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 2 3 4




#pragma pack(push,_CRT_PACKING)
# 35 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
__extension__ typedef unsigned long long size_t;
# 45 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
__extension__ typedef long long ssize_t;






typedef size_t rsize_t;
# 62 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
__extension__ typedef long long intptr_t;
# 75 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
__extension__ typedef unsigned long long uintptr_t;
# 88 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
__extension__ typedef long long ptrdiff_t;
# 98 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
typedef unsigned short wchar_t;







typedef unsigned short wint_t;
typedef unsigned short wctype_t;





typedef int errno_t;




typedef long __time32_t;




__extension__ typedef long long __time64_t;
# 138 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
typedef __time64_t time_t;
# 422 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/crtdefs.h" 3 4
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct,*_locale_t;



typedef struct tagLC_ID {
  unsigned short wLanguage;
  unsigned short wCountry;
  unsigned short wCodePage;
} LC_ID,*LPLC_ID;




typedef struct threadlocaleinfostruct {
  int refcount;
  unsigned int lc_codepage;
  unsigned int lc_collate_cp;
  unsigned long lc_handle[6];
  LC_ID lc_id[6];
  struct {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
  } lc_category[6];
  int lc_clike;
  int mb_cur_max;
  int *lconv_intl_refcount;
  int *lconv_num_refcount;
  int *lconv_mon_refcount;
  struct lconv *lconv;
  int *ctype1_refcount;
  unsigned short *ctype1;
  const unsigned short *pctype;
  const unsigned char *pclmap;
  const unsigned char *pcumap;
  struct __lc_time_data *lc_time_curr;
} threadlocinfo;







#pragma pack(pop)
# 29 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/stdint.h" 2 3 4



# 1 "C:/Program Files/CodeBlocks/MinGW/lib/gcc/x86_64-w64-mingw32/8.1.0/include/stddef.h" 1 3 4
# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/stddef.h" 1 3 4
# 18 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/stddef.h" 3 4
  __attribute__ ((__dllimport__)) extern int *__attribute__((__cdecl__)) _errno(void);

  errno_t __attribute__((__cdecl__)) _set_errno(int _Value);
  errno_t __attribute__((__cdecl__)) _get_errno(int *_Value);


  __attribute__ ((__dllimport__)) extern unsigned long __attribute__((__cdecl__)) __threadid(void);

  __attribute__ ((__dllimport__)) extern uintptr_t __attribute__((__cdecl__)) __threadhandle(void);
# 2 "C:/Program Files/CodeBlocks/MinGW/lib/gcc/x86_64-w64-mingw32/8.1.0/include/stddef.h" 2 3 4
# 33 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/stdint.h" 2 3 4


typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned uint32_t;
__extension__ typedef long long int64_t;
__extension__ typedef unsigned long long uint64_t;


typedef signed char int_least8_t;
typedef unsigned char uint_least8_t;
typedef short int_least16_t;
typedef unsigned short uint_least16_t;
typedef int int_least32_t;
typedef unsigned uint_least32_t;
__extension__ typedef long long int_least64_t;
__extension__ typedef unsigned long long uint_least64_t;





typedef signed char int_fast8_t;
typedef unsigned char uint_fast8_t;
typedef short int_fast16_t;
typedef unsigned short uint_fast16_t;
typedef int int_fast32_t;
typedef unsigned int uint_fast32_t;
__extension__ typedef long long int_fast64_t;
__extension__ typedef unsigned long long uint_fast64_t;


__extension__ typedef long long intmax_t;
__extension__ typedef unsigned long long uintmax_t;
# 10 "C:/Program Files/CodeBlocks/MinGW/lib/gcc/x86_64-w64-mingw32/8.1.0/include/stdint.h" 2 3 4
# 28 "../../../micropython/ports/embed/port/mpconfigport_common.h" 2




# 31 "../../../micropython/ports/embed/port/mpconfigport_common.h"
typedef intptr_t mp_int_t;
typedef uintptr_t mp_uint_t;
typedef long mp_off_t;





# 1 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 1 3
# 11 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
#pragma pack(push,_CRT_PACKING)
# 46 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
  
# 46 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
 typedef struct _heapinfo {
    int *_pentry;
    size_t _size;
    int _useflag;
  } _HEAPINFO;


  extern unsigned int _amblksiz;
# 65 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
       
       
# 76 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
  void *__attribute__((__cdecl__)) calloc(size_t _NumOfElements,size_t _SizeOfElements);
  void __attribute__((__cdecl__)) free(void *_Memory);
  void *__attribute__((__cdecl__)) malloc(size_t _Size);
  void *__attribute__((__cdecl__)) realloc(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _recalloc(void *_Memory,size_t _Count,size_t _Size);


  __attribute__ ((__dllimport__)) void __attribute__((__cdecl__)) _aligned_free(void *_Memory);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_malloc(size_t _Size,size_t _Alignment);


  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_malloc(size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_realloc(void *_Memory,size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_recalloc(void *_Memory,size_t _Count,size_t _Size,size_t _Alignment);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_realloc(void *_Memory,size_t _Size,size_t _Alignment,size_t _Offset);
  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _aligned_offset_recalloc(void *_Memory,size_t _Count,size_t _Size,size_t _Alignment,size_t _Offset);





       
       




void * __mingw_aligned_malloc (size_t _Size, size_t _Alignment);
void __mingw_aligned_free (void *_Memory);
void * __mingw_aligned_offset_realloc (void *_Memory, size_t _Size, size_t _Alignment, size_t _Offset);
void * __mingw_aligned_realloc (void *_Memory, size_t _Size, size_t _Offset);



  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _resetstkoflw (void);
  __attribute__ ((__dllimport__)) unsigned long __attribute__((__cdecl__)) _set_malloc_crt_max_wait(unsigned long _NewValue);

  __attribute__ ((__dllimport__)) void *__attribute__((__cdecl__)) _expand(void *_Memory,size_t _NewSize);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _msize(void *_Memory);






  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _get_sbh_threshold(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _set_sbh_threshold(size_t _NewValue);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _set_amblksiz(size_t _Value);
  __attribute__ ((__dllimport__)) errno_t __attribute__((__cdecl__)) _get_amblksiz(size_t *_Value);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapadd(void *_Memory,size_t _Size);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapchk(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapmin(void);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapset(unsigned int _Fill);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _heapwalk(_HEAPINFO *_EntryInfo);
  __attribute__ ((__dllimport__)) size_t __attribute__((__cdecl__)) _heapused(size_t *_Used,size_t *_Commit);
  __attribute__ ((__dllimport__)) intptr_t __attribute__((__cdecl__)) _get_heap_handle(void);
# 144 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
  static __inline void *_MarkAllocaS(void *_Ptr,unsigned int _Marker) {
    if(_Ptr) {
      *((unsigned int*)_Ptr) = _Marker;
      _Ptr = (char*)_Ptr + 16;
    }
    return _Ptr;
  }
# 163 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
  static __inline void __attribute__((__cdecl__)) _freea(void *_Memory) {
    unsigned int _Marker;
    if(_Memory) {
      _Memory = (char*)_Memory - 16;
      _Marker = *(unsigned int *)_Memory;
      if(_Marker==0xDDDD) {
 free(_Memory);
      }





    }
  }
# 209 "C:/Program Files/CodeBlocks/MinGW/x86_64-w64-mingw32/include/malloc.h" 3
#pragma pack(pop)
# 40 "../../../micropython/ports/embed/port/mpconfigport_common.h" 2
# 8 "./mpconfigport.h" 2
# 92 "../../../micropython/py/mpconfig.h" 2
# 842 "../../../micropython/py/mpconfig.h"

# 842 "../../../micropython/py/mpconfig.h"
typedef float mp_float_t;
# 30 "<stdin>" 2





QCFG(BYTES_IN_LEN, (1))
QCFG(BYTES_IN_HASH, (0))

Q()
Q(*)
Q(_)
Q(/)
# 50 "<stdin>"
Q({:#o})
Q({:#x})

Q({:#b})
Q( )
Q(\n)
Q(maximum recursion depth exceeded)
Q(<module>)
Q(<lambda>)
Q(<listcomp>)
Q(<dictcomp>)
Q(<setcomp>)
Q(<genexpr>)
Q(<string>)
Q(<stdin>)
Q(utf-8)
# 74 "<stdin>"
Q(simple_sensor)
Q(A1)

Q(A2)

Q(A3)

Q(A4)

Q(A5)

Q(A6)

Q(A7)

Q(A8)

Q(Analog)

Q(ArithmeticError)

Q(ArithmeticError)

Q(AssertionError)

Q(AssertionError)

Q(AssertionError)

Q(AttributeError)

Q(AttributeError)

Q(B1)

Q(B10)

Q(B11)

Q(B12)

Q(B2)

Q(B3)

Q(B4)

Q(B5)

Q(B6)

Q(B7)

Q(B8)

Q(B9)

Q(BRAKE_MODE)

Q(BaseException)

Q(BaseException)

Q(C1)

Q(EOFError)

Q(EOFError)

Q(EV3Touch)

Q(EV3Ultrasonic)

Q(EV3Ultrasonic)

Q(Ellipsis)

Q(Ellipsis)

Q(Exception)

Q(Exception)

Q(FORWARD)

Q(GeneratorExit)

Q(GeneratorExit)

Q(I2C)

Q(ImportError)

Q(ImportError)

Q(IndentationError)

Q(IndentationError)

Q(IndexError)

Q(IndexError)

Q(KeyError)

Q(KeyError)

Q(Key_Constants)

Q(KeyboardInterrupt)

Q(KeyboardInterrupt)

Q(LookupError)

Q(LookupError)

Q(M1)

Q(M2)

Q(M3)

Q(M4)

Q(MOVETO_MODE)

Q(MemoryError)

Q(MemoryError)

Q(NO_KEY)

Q(NameError)

Q(NameError)

Q(NoneType)

Q(NotImplementedError)

Q(NotImplementedError)

Q(OSError)

Q(OSError)

Q(OverflowError)

Q(OverflowError)

Q(POWER_MODE)

Q(REVERSE)

Q(RuntimeError)

Q(RuntimeError)

Q(S1)

Q(S1)

Q(S2)

Q(S2)

Q(S3)

Q(S4)

Q(SPEED_MODE)

Q(StopIteration)

Q(StopIteration)

Q(SyntaxError)

Q(SyntaxError)

Q(SystemExit)

Q(SystemExit)

Q(TOF)

Q(Touch)

Q(TypeError)

Q(TypeError)

Q(UART)

Q(ValueError)

Q(ValueError)

Q(ZeroDivisionError)

Q(ZeroDivisionError)

Q(_0x0a_)

Q(__add__)

Q(__bool__)

Q(__build_class__)

Q(__call__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__class__)

Q(__complex__)

Q(__contains__)

Q(__delitem__)

Q(__delitem__)

Q(__enter__)

Q(__eq__)

Q(__eq__)

Q(__exit__)

Q(__float__)

Q(__ge__)

Q(__getattr__)

Q(__getattr__)

Q(__getitem__)

Q(__getitem__)

Q(__getitem__)

Q(__getitem__)

Q(__gt__)

Q(__hash__)

Q(__iadd__)

Q(__import__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__init__)

Q(__int__)

Q(__isub__)

Q(__iter__)

Q(__le__)

Q(__len__)

Q(__lt__)

Q(__main__)

Q(__main__)

Q(__module__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__name__)

Q(__ne__)

Q(__new__)

Q(__new__)

Q(__next__)

Q(__next__)

Q(__next__)

Q(__next__)

Q(__qualname__)

Q(__repl_print__)

Q(__repl_print__)

Q(__repr__)

Q(__repr__)

Q(__setitem__)

Q(__setitem__)

Q(__str__)

Q(__sub__)

Q(__traceback__)

Q(_brace_open__colon__hash_b_brace_close_)

Q(_brace_open__colon__hash_o_brace_close_)

Q(_brace_open__colon__hash_x_brace_close_)

Q(_lt_dictcomp_gt_)

Q(_lt_dictcomp_gt_)

Q(_lt_genexpr_gt_)

Q(_lt_genexpr_gt_)

Q(_lt_lambda_gt_)

Q(_lt_lambda_gt_)

Q(_lt_listcomp_gt_)

Q(_lt_listcomp_gt_)

Q(_lt_module_gt_)

Q(_lt_module_gt_)

Q(_lt_setcomp_gt_)

Q(_lt_setcomp_gt_)

Q(_lt_string_gt_)

Q(_space_)

Q(_star_)

Q(_star_)

Q(abs)

Q(acc)

Q(acc)

Q(all)

Q(angle)

Q(any)

Q(append)

Q(args)

Q(bin)

Q(bool)

Q(bool)

Q(bound_method)

Q(brake)

Q(builtins)

Q(builtins)

Q(button)

Q(button)

Q(bytecode)

Q(bytes)

Q(bytes)

Q(callable)

Q(chr)

Q(classmethod)

Q(classmethod)

Q(clear)

Q(clear)

Q(clearMemory)

Q(close)

Q(close)

Q(closure)

Q(collect)

Q(complex)

Q(complex)

Q(config)

Q(config)

Q(copy)

Q(copy)

Q(count)

Q(count)

Q(devices)

Q(devices)

Q(dict)

Q(dict)

Q(dict_view)

Q(dir)

Q(direction)

Q(direction)

Q(disable)

Q(divmod)

Q(enable)

Q(end)

Q(endswith)

Q(errno)

Q(eval)

Q(exec)

Q(extend)

Q(find)

Q(float)

Q(float)

Q(format)

Q(from_bytes)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(function)

Q(gc)

Q(gc)

Q(generator)

Q(generator)

Q(get)

Q(get)

Q(get)

Q(get)

Q(get)

Q(getDigital)

Q(getKey)

Q(getMemory)

Q(getPosition)

Q(getPower)

Q(getSpeed)

Q(getTime)

Q(getValueExt)

Q(getattr)

Q(globals)

Q(hasattr)

Q(hash)

Q(hex)

Q(id)

Q(id)

Q(imag)

Q(index)

Q(index)

Q(index)

Q(insert)

Q(int)

Q(int)

Q(isalpha)

Q(isdigit)

Q(isenabled)

Q(isinstance)

Q(islower)

Q(isspace)

Q(issubclass)

Q(isupper)

Q(items)

Q(iter)

Q(iterator)

Q(iterator)

Q(iterator)

Q(iterator)

Q(join)

Q(key)

Q(keys)

Q(keys)

Q(keys)

Q(ki)

Q(ki)

Q(kp)

Q(kp)

Q(len)

Q(list)

Q(list)

Q(little)

Q(little)

Q(locals)

Q(lower)

Q(lstrip)

Q(map)

Q(map)

Q(mem_alloc)

Q(mem_free)

Q(mem_offset)

Q(mem_offset)

Q(memory)

Q(memory)

Q(micropython)

Q(mode)

Q(mode)

Q(mode)

Q(module)

Q(monitor)

Q(monitor)

Q(monitor_keys)

Q(motor)

Q(motor)

Q(next)

Q(object)

Q(object)

Q(oct)

Q(ord)

Q(pop)

Q(pop)

Q(popitem)

Q(port)

Q(port)

Q(port)

Q(position)

Q(pow)

Q(print)

Q(range)

Q(range)

Q(range)

Q(real)

Q(remove)

Q(replace)

Q(repr)

Q(reverse)

Q(reverse)

Q(rfind)

Q(rindex)

Q(round)

Q(rsplit)

Q(rstrip)

Q(send)

Q(send)

Q(sensor)

Q(sensor)

Q(sep)

Q(servo)

Q(servo)

Q(set)

Q(set)

Q(setMemory)

Q(setText)

Q(setattr)

Q(setdefault)

Q(sort)

Q(sorted)

Q(speed)

Q(speed)

Q(split)

Q(startswith)

Q(staticmethod)

Q(staticmethod)

Q(str)

Q(str)

Q(strip)

Q(sum)

Q(super)

Q(super)

Q(super)

Q(throw)

Q(throw)

Q(ticks)

Q(ticks)

Q(time)

Q(time)

Q(to_bytes)

Q(tuple)

Q(tuple)

Q(type)

Q(type)

Q(type)

Q(type)

Q(update)

Q(upper)

Q(value)

Q(values)

Q(wait)

Q(zip)

Q(zip)
