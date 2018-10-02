// Copyright 27-Sept-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Gen.h"
#include "math.h"
#include "DEFS.h"
#include "dmc/rnd.h"
#include "dmc/ct/Ajson.h"

struct gen_Gen {
  int size;
  double *gens;
};

Gen *gen_new(int size, double *gens) {
  double *gs = ATOMIC(size * sizeof(gens));
  double *pgs = gs;
  REPEAT(size) {
    *pgs++ = *gens++;
  }_REPEAT
  Gen *this = MALLOC(Gen);
  this->size = size;
  this->gens = gs;
  return this;
}

Gen *gen_mutate(Gen *this) {
  int size = this->size;
  double *gens = ATOMIC(size * sizeof(double));
  double *p = gens;
  double *old_p = this->gens;
  REPEAT(size) {
    *p++ = *old_p++;
  }_REPEAT
  do {
    int ix = rnd_i(size);
    gens[ix] = gens[ix] * MUTATION_MULTIPLIER;
  } while(rnd_i(2));
  return gen_new(size, gens);
}

Json *gen_to_json(Gen *this) {
  Ajson *serial = ajson_new();
  jarr_aint(serial, this->size);
  double *p = this->gens;
  REPEAT(this->size) {
    jarr_adouble(serial, *p++,  4);
  }_REPEAT
  return json_warray(serial);
}

Gen *gen_from_json(Json *js) {
  Ajson *serial = json_rarray(js);
  size_t i = 0;
  int size = jarr_gint(serial, i++);
  double *gens = ATOMIC(size * sizeof(double));
  double *p = gens;
  REPEAT(size) {
    *p++ = jarr_gdouble(serial, i++);
  }_REPEAT
  return gen_new(size, gens);
}

double gen_get(Gen *this, int ix) {
  return this->gens[ix];
}

double gen_proximity(Gen *this, Gen *other) {
  int s1 = this->size;
  int s2 = other->size;

  if (s1 != s2) {
    return 0;
  }

  double *gens1 = this->gens;
  double *gens2 = other->gens;
  double r = 1;
  double g1, g2, d0;
  REPEAT(s1) {
    g1 = *gens1++;
    g2 = *gens2++;
    if (g1 > g2) {
      d0 = (g1 - g2) / g1;
    } else if (g1 < g2) {
      d0 = (g2 - g1) / g2;
    } else {
      d0 = 1;
    }
    r += d0 * d0;
  }_REPEAT

  double prox = (s1 - sqrt(r)) / s1;
  double prox10 = prox * 10;
  r = 0.9995 - asinh(pow(prox10, 3)) / (asinh(prox10) * 2.55);
  if (r > 1) {
    puts("gen_proximity > 1");
    return r;
  }
  if (r < 0) {
    puts("gen_proximity < 0");
    return 0;
  }
  return r;
}
