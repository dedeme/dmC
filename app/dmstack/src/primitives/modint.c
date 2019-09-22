// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modint.h"
#include "dmc/rnd.h"
#include "fails.h"
#include "Machine.h"

static void fn2 (Machine *m, int (*f)(int, int)) {
  int n2 = token_int(machine_pop_exc(m, token_INT));
  int n1 = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_new_int(0, f(n1, n2)));
}

static void fromStr (Machine *m) {
  machine_push(m, token_new_int(
    0, js_ri((Js *)token_string(machine_pop_exc(m, token_STRING)))
  ));
}

static void sabs (Machine *m) {
  int i = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_new_int(0, i >= 0 ? i : -i));
}

static void rnd (Machine *m) {
  machine_push(m, token_new_int(
    0, rnd_i(token_int(machine_pop_exc(m, token_INT)))
  ));
}

static void sdiv (Machine *m) {
  int den = token_int(machine_pop_exc(m, token_INT));
  int num = token_int(machine_pop_exc(m, token_INT));
  div_t r = div(num, den);
  machine_push(m, token_new_int(0, r.quot));
  machine_push(m, token_new_int(0, r.rem));
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

  map_put(r, "fromStr", fromStr); // STRING - INT
  map_put(r, "abs", sabs); // INT - INT
  map_put(r, "rnd", rnd); // [] - INT
  map_put(r, "div", sdiv); // [INT, INT] - [INT, INT]
                               // (num, den) - [quot, rem]
  map_put(r, "max", max); // [INT, INT] - INT
  map_put(r, "min", min); // [INT, INT] - INT

  return r;
}


