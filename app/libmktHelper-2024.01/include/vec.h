// Copyright 28-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Vector and matrix utilities.

#ifndef VEC_H
  #define VEC_H

#include "kut/arr.h"

typedef struct vec_Vec {
  int size;
  double *vs;
} Vec;

/// Constructor.
Vec *vec_new (int size, double *vs);

/// 'this' is converted to [n.] and JSONized with 'dec' decimal positions.
char *vec_to_js (Vec *this, int dec);

/// 'js' is [n.] JSONized.
Vec *vec_from_js (char *js);

/// JSONize an array of Vec converted to [[n.].] with 'dec' decimal positions.
char *vec_matrix_to_js (Arr *mtx, int dec);

/// Returns an Arr<Vec>. 'js' is [[n.].] JSONized.
Arr *vec_matrix_from_js (char *js);

/// Returns a duplicate of 'values' array in a new one.
double *vec_dup (int size, double *values);

#endif
