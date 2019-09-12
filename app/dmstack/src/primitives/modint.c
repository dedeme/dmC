// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modint.h"
#include "dmc/rnd.h"
#include "fails.h"
#include "Machine.h"

static void fn2 (Machine *m, int (*f)(int, int)) {
  int n2 = token_int(machine_pop_exc(m, token_INT));
  int n1 = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_new_int(f(n1, n2)));
}

static void fromStr (Machine *m) {
  machine_push(m, token_new_int(
    js_ri((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void sabs (Machine *m) {
  int i = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_new_int(i >= 0 ? i : -i));
}

static void rnd (Machine *m) {
  machine_push(m, token_new_int(
    rnd_i(token_int(machine_pop_exc(m, token_INT)))
  ));
}

static void sdiv (Machine *m) {
  int den = token_int(machine_pop_exc(m, token_INT));
  int num = token_int(machine_pop_exc(m, token_INT));
  div_t r = div(num, den);
  machine_push(m, token_new_int(r.quot));
  machine_push(m, token_new_int(r.rem));
}

static void max (Machine *m) {
  int mx (int n1, int n2) { return n1 >= n2 ? n1 : n2; }
  fn2(m, mx);
}

static void min (Machine *m) {
  int mn (int n1, int n2) { return n1 <= n2 ? n1 : n2; }
  fn2(m, mn);
}

// Resturns Map<primitives_Fn>
Map *modint_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "int.fromStr", fromStr);
  map_put(r, "int.abs", sabs);
  map_put(r, "int.rnd", rnd);
  map_put(r, "int.div", sdiv);
  map_put(r, "int.max", max);
  map_put(r, "int.min", min);

  return r;
}


