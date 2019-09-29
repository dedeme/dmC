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
    0, js_rl((Js *)token_string(machine_pop_exc(m, token_STRING)))
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

Pmodule *modint_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("fromStr", fromStr); // STRING - INT
  add("abs", sabs); // INT - INT
  add("rnd", rnd); // [] - INT
  add("div", sdiv); // [INT, INT] - [INT, INT]
                               // (num, den) - [quot, rem]
  add("max", max); // [INT, INT] - INT
  add("min", min); // [INT, INT] - INT

  return r;
}


