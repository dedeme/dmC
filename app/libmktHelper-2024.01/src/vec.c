// Copyright 28-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "vec.h"
#include "kut/DEFS.h"
#include "kut/js.h"

Vec *vec_new (int size, double *vs) {
  Vec *this = MALLOC(Vec);
  this->size = size;
  this->vs = vs;
  return this;
}

char *vec_to_js (Vec *this, int dec) {
  double *pvs = this->vs;
  //<char>
  Arr *r = arr_new_bf(this->size);
  REPEAT(this->size) {
    arr_push(r, js_wf(*pvs++, dec));
  }_REPEAT
  return js_wa(r);
}

Vec *vec_from_js (char *js) {
  // <char>
  Arr *a = js_ra(js);
  int size = arr_size(a);
  double *vs = ATOMIC(size * sizeof(double));
  double *pvs = vs;
  EACH(a, char, n) {
    *pvs++ = js_rd(n);
  }_EACH
  return vec_new(size, vs);
}

char *vec_matrix_to_js (Arr *mtx, int dec) {
  // <char>
  Arr *r = arr_new_bf(arr_size(mtx));
  EACH(mtx, Vec, v) {
    arr_push(r, vec_to_js(v, dec));
  }_EACH
  return js_wa(r);
}

// <Vec>
Arr *vec_matrix_from_js (char *js) {
  // <char> passed to <Vec>!
  Arr *a = js_ra(js);
  void **p = arr_begin(a);
  Vec *v = vec_from_js((char *)*p);
  int size = v->size;
  *p++ = v;
  RANGE(i, 1, arr_size(a)) {
    Vec *v = vec_from_js((char *)*p);
    if (size != v->size)
      EXC_ILLEGAL_STATE(str_f(
        "File 0 of matrix has %d values, but file %d has %d",
        size, i, v->size
      ));
    *p++ = v;
  }_RANGE
  return a;
}

double *vec_dup (int size, double *values) {
  double *new_values = ATOMIC(size * sizeof(double));
  memcpy(new_values, values, size * sizeof(double));
  return new_values;
}
