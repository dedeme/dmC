// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modfloat.h"
#include <math.h>
#include "dmc/rnd.h"
#include "fails.h"
#include "tk.h"

static void fn2 (Machine *m, double (*f)(double, double)) {
  double n2 = tk_pop_float(m);
  double n1 = tk_pop_float(m);
  machine_push(m, token_new_float(0, f(n1, n2)));
}

static void fromStr (Machine *m) {
  machine_push(m, token_new_float(
    0, js_rd((Js *)tk_pop_string(m))
  ));
}

static void sabs (Machine *m) {
  double d = tk_pop_float(m);
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
    0, round(tk_pop_float(m))
  ));
}

static void roundn (Machine *m) {
  Int scale = tk_pop_int(m);
  fails_range(m, 0, 9, scale);
  double n = tk_pop_float(m);
  int mul = 1;
  REPEAT(scale) {
    mul *= 10;
  }_REPEAT

  machine_push(m, token_new_float(0, round(n * mul) / mul));
}

static void eq (Machine *m) {
  double gap = tk_pop_float(m);
  double n1 = tk_pop_float(m);
  double n2 = tk_pop_float(m);
  machine_push(m, token_new_int(0, n2 < n1 + gap && n2 > n1 - gap));
}

static void toint (Machine *m) {
  machine_push(m, token_new_int(
    0, (Int)tk_pop_float(m)
  ));
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

  add("round", sround); // FLOAT - FLOAT
  add("roundn", roundn); // [FLOAT, INT] - FLOAT
  add("==", eq);

  add("toInt", toint); // [FLOAT] - INT
  return r;
}

