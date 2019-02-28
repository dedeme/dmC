// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Gen.h"
#include "dmc/Dec.h"
#include "dmc/rnd.h"
#include "DEFS.h"

Gen *gen_new(int n) {
  Darr *this = darr_bf_new(n);
  REPEAT(n)
    darr_push(this, 0.5);
  _REPEAT
  return this;
}

void gen_free(Gen *this) {
  darr_free(this);
}

int gen_n(Gen *this) {
  return darr_size(this);
}

double *gen_values(Gen *this) {
  return darr_start(this);
}

int gen_eq(Gen *g1, Gen *g2) {
  int size = darr_size(g1);
  if (size == darr_size(g2)) {
    double *p1 = darr_start(g1);
    double *p2 = darr_start(g2);
    REPEAT(size)
      if (!dec_eq_gap(*p1++, *p2++, 0.0000001)) {
        return 0;
      }
    _REPEAT
    return 1;
  }
  return 0;
}

Gen *gen_mutate_new(Gen *this) {
  int size = darr_size(this);
  Darr *rs = darr_bf_new(size);
  double *p = darr_start(this);
  REPEAT(size)
    double mul = MUTATION_MULTIPLIER * (rnd_d() * 2 - 1);
    double n = *p++;
    darr_push(rs, mul <= 0 ? n * (1 + mul) : n + (1 - n) * mul);
  _REPEAT
  return rs;
}

Gen *gen_copy_new(Gen *this) {
  Darr *r = darr_bf_new(darr_size(this));
  DEACH((Darr *)this, d)
    darr_push(r, d);
  _EACH
  return (Gen *)r;
}

Js *gen_to_js_new(Gen *this) {
  return darr_to_js_new(this);
}

///
Gen *gen_from_js_new(Js *js) {
  return darr_from_js_new(js);
}
