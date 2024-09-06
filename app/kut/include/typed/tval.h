// Copyright 20-Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Typed values.

#ifndef TYPED_TVAL_H
  #define TYPED_TVAL_H

#include <stdint.h>

typedef struct tarr_Arri Arri;
typedef struct tarr_Arrf Arrf;
typedef struct tarr_Arrs Arrs;

/// Type value.
typedef union tval_Tval {
    int b;
    int64_t i;
    double f;
    char *s;
    Arri *I;
    Arrf *F;
    Arrs *S;
  } Tval;

#endif
