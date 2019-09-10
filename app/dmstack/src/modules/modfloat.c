// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modfloat.h"
#include "dmc/rnd.h"
#include "fails.h"
#include "Machine.h"

static void fn2 (Machine *m, double (*f)(double, double)) {
  double n2 = token_float(machine_pop_exc(m, token_FLOAT));
  double n1 = token_float(machine_pop_exc(m, token_FLOAT));
  machine_push(m, token_new_float(f(n1, n2)));
}

static void fromStr (Machine *m) {
  machine_push(m, token_new_float(
    js_rd((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void sabs (Machine *m) {
  double d = token_float(machine_pop_exc(m, token_FLOAT));
  machine_push(m, token_new_float(d >= 0 ? d : -d));
}

static void rnd (Machine *m) {
  machine_push(m, token_new_float(rnd_d()));
}

static void max (Machine *m) {
  double mx (double n1, double n2) { return n1 >= n2 ? n1 : n2; }
  fn2(m, mx);
}

static void min (Machine *m) {
  double mn (double n1, double n2) { return n1 <= n2 ? n1 : n2; }
  fn2(m, mn);
}

// Resturns Map<primitives_Fn>
Map *modfloat_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "float.fromStr", fromStr);
  map_put(r, "float.abs", sabs);
  map_put(r, "float.rnd", rnd);
  map_put(r, "float.max", max);
  map_put(r, "float.min", min);

  return r;
}

