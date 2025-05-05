  #define GC_THREADS
  #define __STDC_WANT_LIB_EXT2__ 1  //Define for asprintf
  #define _GNU_SOURCE // Need for environ

  #define TRUE 1
  #define FALSE 0
  #define MALLOC(type) (type *)GC_MALLOC(sizeof(type))
  #define ATOMIC(size) GC_MALLOC_ATOMIC(size)
  #define REPEAT(n) { \
    int __i = (n) + 1; \
    while (--__i) {
  #define _REPEAT }}
  #define RANGE0(i, end) { \
    int __end = end; \
    int i = -1; \
    while (++i < __end) {
  #define RANGE(i, begin, end) { \
    int __end = end; \
    int i = (begin) - 1; \
    while (++i < __end) {
  #define _RANGE }}
  #define EACH(a, type, e) { \
    Arr *__a = (Arr *)a; \
    void **__p = arr_begin(__a); \
    void **__pend = arr_end(__a); \
    type *e; \
    int _i = -1; \
    while(__p < __pend) { \
      ++_i; \
      e = *__p++;
  #define EACHR(a, type, e) { \
    Arr *__a = (Arr *)a; \
    void **__p = arr_end(__a); \
    void **__pstart = arr_begin(__a); \
    type *e; \
    int _i = arr_size(__a); \
    while(__p > __pstart) { \
      --_i; \
      e = *(--__p);
  #define _EACH }}
  #define TRY { \
    jmp_buf *__TRY_buf = MALLOC(jmp_buf); \
    exc_add(__TRY_buf); \
    if (!setjmp(*__TRY_buf)) {
  #define CATCH(e) ;exc_remove();} else { exc_remove(); Exc *e = exc_get();
  #define _TRY }}
  #define THROW(type, msg) \
      exc_throw(type, msg, __FILE__, (char *)__func__, __LINE__)
  #define EXC_GENERIC(msg) \
    THROW(exc_generic_t, msg)
  #define EXC_RANGE(value, min, max) { \
      int __v = value; \
      int __min = min; \
      int __max = max; \
      if (__v < __min || __v > __max) \
        THROW(exc_range_t, exc_range(__min, __max, __v)); \
    }
  #define EXC_ILLEGAL_ARGUMENT(msg, expected, actual) \
    THROW(exc_illegal_argument_t, exc_illegal_argument(msg, expected, actual))
  #define EXC_ILLEGAL_STATE(msg) \
    THROW(exc_illegal_state_t, msg)
  #define EXC_IO(msg) \
    THROW(exc_io_t, msg)
  #define TEST(actual, expected) { \
      char *__actual = actual; \
      char *__expected = expected; \
      if (strcmp(__actual, __expected)) \
      EXC_ILLEGAL_ARGUMENT("Test failed", __expected, __actual); \
    }
  #define TESTI(actual, expected) { \
      char *__sactual = str_f("%d", actual); \
      char *__sexpected = str_f("%d", expected); \
      if (strcmp(__sactual, __sexpected)) \
      EXC_ILLEGAL_ARGUMENT("Test failed", __sexpected, __sactual); \
    }
  #define TESTF(actual, expected) { \
      char *__sactual = str_f("%.10f", (double)actual); \
      char *__sexpected = str_f("%.10f", (double)expected); \
      if (strcmp(__sactual, __sexpected)) \
      EXC_ILLEGAL_ARGUMENT("Test failed", __sexpected, __sactual); \
    }
  #define exc_generic_t "generic"
  #define exc_range_t "range"
  #define exc_illegal_argument_t "argument"
  #define exc_illegal_state_t "state"
  #define exc_io_t "io"

  #include <gc.h>
  #include <pthread.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <wctype.h>
  #include <ctype.h>
  #include <locale.h>
  #include <regex.h>
  #include <assert.h>
  #include <setjmp.h>
  #include <stdint.h>
  #include <time.h>
  #include <sys/time.h>
  #include <dirent.h>
  #include <errno.h>
  #include <dirent.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <stdarg.h>
  #include <execinfo.h>
  #include <unistd.h>
  #include <limits.h>
  #include <float.h>
  #include <math.h>
  #include <sys/types.h>
  #include <pwd.h>
  #include <signal.h>
  #include <netdb.h>
  #include <arpa/inet.h>
  #include <sys/socket.h>
  #include <netinet/in.h>

  typedef void *(*FMAP)(void *);
  typedef int (*FEQ)(void *, void *);
  typedef void (*FPROC)(void *);
  typedef int (*FPRED)(void *);
  typedef char *(*FTO)(void *);
  typedef void *(*FFROM)(char *js);
  typedef struct opt_Opt Opt;
  typedef struct it_It It;
  typedef struct arr_Arr Arr;
  typedef struct bytes_Bytes Bytes;
  typedef struct buf_Buf Buf;
  typedef struct arr_Arr Arr;
  typedef struct exc_Exc Exc;
  typedef int64_t Time;
  typedef struct file_File File;
  typedef struct tp_Tp Tp;
  typedef struct tp3_Tp3 Tp3;
  typedef struct it_It It;
  typedef struct kv_Kv Kv;
  typedef struct arr_Map Map;
  typedef struct regex_RegexOffset RegexOffset;
  typedef struct rs_Rs Rs;
  typedef struct tcp_TcpServer TcpServer;
  typedef struct tcp_TcpConn TcpConn;
