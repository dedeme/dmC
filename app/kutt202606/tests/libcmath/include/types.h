
#ifndef TYPES_H
  #define TYPES_H

#include <stdint.h>

typedef union built_Val Val;
typedef struct __arr_Varr Varr;

union built_Val {
  int b;
  int64_t i;
  double f;
  char *s;
  Varr *a;
  void *o;
};

struct __arr_Varr {
  Val *begin;
  Val *end;
  Val *end_bf;
};

#endif
