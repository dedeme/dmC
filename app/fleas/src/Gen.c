// Copyright 26-Oct-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Gen.h"
#include "DEFS.h"
#include "dmc/rnd.h"

Gen *gen_new(void) {
  double *this = malloc(3 * sizeof(double));
  this[0] = 0.5;
  this[1] = 0.5;
  this[2] = 0.5;
  return (Gen *) this;
}

Gen *gen_values_new(double days, double buy_strip, double sell_strip) {
  double *this = malloc(3 * sizeof(double));
  this[0] = days;
  this[1] = buy_strip;
  this[2] = sell_strip;
  return (Gen *) this;
}

Gen *gen_copy_new(Gen *this) {
  double *r = malloc(3 * sizeof(double));
  r[0] = ((double *)this)[0];
  r[1] = ((double *)this)[1];
  r[2] = ((double *)this)[2];
  return (Gen *) r;
}

int gen_eq(Gen *this, Gen *other) {
  return  ((double *)this)[0] == ((double *)other)[0] &&
          ((double *)this)[1] == ((double *)other)[1] &&
          ((double *)this)[2] == ((double *)other)[2];
}

double gen_get(Gen *this, enum gen_Ix ix) {
  return ((double *)this)[ix];
}

void gen_set(Gen *this, enum gen_Ix ix, double value) {
  ((double *)this)[ix] = value;
}

Gen *gen_mutate_new(Gen *this) {
  Gen *r = gen_new();
  double mul, val;
  RANGE0(i, 3)
    mul = MUTATION_MULTIPLIER * (rnd_d() * 2 - 1);
    val = ((double *)this)[i];
    ((double *)r)[i] = mul <= 0 ? val * (1 + mul) : val + (1 - val) * mul;
  _RANGE
  return r;
}

double gen_proximity(Gen *this, Gen *other) {
  double sum = 0;
  double s1, s2;
  RANGE0(i, 3)
    s1 = ((double *)this)[i];
    s2 = ((double *)other)[i];
    sum += s1 > s2 ? s1 - s2 : s2 - s1;
  _RANGE
  return 1 - sum / 3;
}

Js *gen_to_js_new(Gen *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_wd_new(((double *)this)[0], 6));
  arr_push(a, js_wd_new(((double *)this)[1], 6));
  arr_push(a, js_wd_new(((double *)this)[2], 6));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Gen *gen_from_js_new(Js *js) {
  Gen *this = gen_new();
  // Arr[Js]
  Arr *a = js_ra_new(js);
  ((double *)this)[0] = js_rd(arr_get(a, 0));
  ((double *)this)[1] = js_rd(arr_get(a, 1));
  ((double *)this)[2] = js_rd(arr_get(a, 2));
  arr_free(a);
  return this;
}
