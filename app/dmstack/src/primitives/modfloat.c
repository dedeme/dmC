// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modfloat.h"
#include <math.h>
#include "dmc/rnd.h"
#include "fails.h"
#include "Machine.h"

static void fn2 (Machine *m, double (*f)(double, double)) {
  double n2 = token_float(machine_pop_exc(m, token_FLOAT));
  double n1 = token_float(machine_pop_exc(m, token_FLOAT));
  machine_push(m, token_new_float(0, f(n1, n2)));
}

static void fromStr (Machine *m) {
  machine_push(m, token_new_float(
    0, js_rd((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void sabs (Machine *m) {
  double d = token_float(machine_pop_exc(m, token_FLOAT));
  machine_push(m, token_new_float(0, d >= 0 ? d : -d));
}

static void rnd (Machine *m) {
  machine_push(m, token_new_float(0, rnd_d()));
}

static void max (Machine *m) {
  double mx (double n1, double n2) { return n1 >= n2 ? n1 : n2; }
  fn2(m, mx);
}

static void min (Machine *m) {
  double mn (double n1, double n2) { return n1 <= n2 ? n1 : n2; }
  fn2(m, mn);
}

static void sround (Machine *m) {
  machine_push(m, token_new_float(
    0, round(token_float(machine_pop_exc(m, token_FLOAT)))
  ));
}

static void roundn (Machine *m) {
  int scale = token_float(machine_pop_exc(m, token_INT));
  fails_range(m, 0, 9, scale);
  double n = token_float(machine_pop_exc(m, token_FLOAT));
  int mul = 1;
  REPEAT(scale) {
    mul *= 10;
  }_REPEAT

  machine_push(m, token_new_float(0, round(n * mul) / mul));
}

static void eq (Machine *m) {
  double gap = token_float(machine_pop_exc(m, token_FLOAT));
  double n1 = token_float(machine_pop_exc(m, token_FLOAT));
  double n2 = token_float(machine_pop_exc(m, token_FLOAT));
  machine_push(m, token_new_int(0, n2 < n1 + gap && n2 > n1 - gap));
}

Pmodule *modfloat_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("fromStr", fromStr); // STRING - FLOAT
  add("abs", sabs); // FLOAT - FLOAT
  add("rnd", rnd); //  [] - FLOAT
  add("max", max); // [FLOAT, FLOAT] - FLOAT
  add("min", min); // [FLOAT, FLOAT] - FLOAT

  add("round", sround);
  add("roundn", roundn);
  add("==", eq);

  return r;
}

