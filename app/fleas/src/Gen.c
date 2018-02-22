// Copyright 16-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Gen.h"
#include <dm.h>
#include "math.h"

struct gen_Gen {
  uint max;
  uint actual;
};

Gen *gen_new(uint maxOptions) {
  Gen *this = MALLOC(Gen);
  this->max = maxOptions;
  this->actual = (uint)rnd_i(maxOptions);
  return this;
}

Gen *gen_copy(Gen *this) {
  Gen *other = MALLOC(Gen);
  other->max = this->max;
  other->actual = this->actual;
  return other;
}

Gen *gen_mutate(Gen *this) {
  uint max = this->max;
  uint actual = this->actual;

  uint rnd = rnd_i(1275);
  double p50 = (-1 + sqrt(1 + 8 * rnd)) / 2;
  double p = 1 - p50 / 50;

  int range = rnd_i(2)
    ? max - actual - 1
    : -actual
  ;

  Gen *other = MALLOC(Gen);
  other->max = this->max;
  other->actual = actual + (int)round(range * p);
  return other;
}

inline
uint gen_max(Gen *this) {
  return this->max;
}

inline
uint gen_actual(Gen *this) {
  return this->actual;
}

Json *gen_serialize(Gen *this) {
  Arr/*Json*/ *serial = arr_new();

  jarr_auint(serial, this->max);
  jarr_auint(serial, this->actual);

  return json_warray(serial);
}

Gen *gen_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Gen *this = MALLOC(Gen);

  size_t i = 0;
  this->max = jarr_guint(serial, i++);
  this->actual = jarr_guint(serial, i++);

  return this;
}
