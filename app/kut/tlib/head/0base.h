#define _GNU_SOURCE
#define GC_THREADS
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <wctype.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <setjmp.h>
#include <regex.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <gc.h>
#define TRUE 1
#define FALSE 0
#define MALLOC(type) (type *)GC_MALLOC(sizeof(type))
#define ATOMIC(size) GC_MALLOC_ATOMIC(size)

typedef struct tarr_Arri Arri;
typedef struct tarr_Arrf Arrf;
typedef struct tarr_Arrs Arrs;
typedef struct obj_Obj Obj;

static void (*built_exc_fns) (char, void *);

static int built_eqb (int v1, int v2);
static int built_eqi (int64_t v1, int64_t v2);
static int built_eqf (double v1, double v2);
static int built_eqs (char *v1, char *v2);
static int built_eqo (Obj *v1, Obj *v2);
static int built_eqI (Arri *v1, Arri *v2);
static int built_eqF (Arrf *v1, Arrf *v2);
static int built_eqS (Arrs *v1, Arrs *v2);
static int built_eqD (Arrs *v1, Arrs *v2);
static int built_neqb (int v1, int v2);
static int built_neqi (int64_t v1, int64_t v2);
static int built_neqf (double v1, double v2);
static int built_neqs (char *v1, char *v2);
static int built_neqo (Obj *v1, Obj *v2);
static int built_neqI (Arri *v1, Arri *v2);
static int built_neqF (Arrf *v1, Arrf *v2);
static int built_neqS (Arrs *v1, Arrs *v2);
static int built_neqD (Arrs *v1, Arrs *v2);
static char *built_to_strb (int v);
static char *built_to_stri (int64_t v);
static char *built_to_strf (double s);
static char *built_to_strs (char *s);
static char *built_to_stro (Obj *o);
static char *built_to_strI (Arri *a);
static char *built_to_strF (Arrf *a);
static char *built_to_strS (Arrs *a);
static char *built_to_strD (Arrs *m);
static void built_trace(int is_short, char *position, char *exp);
