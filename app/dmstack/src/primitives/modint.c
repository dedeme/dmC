// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modint.h"
#include <limits.h>
#include "dmc/rnd.h"
#include "fails.h"
#include "tk.h"

static void fn2 (Machine *m, int (*f)(int, int)) {
  Int n2 = tk_pop_int(m);
  Int n1 = tk_pop_int(m);
  machine_push(m, token_new_int(f(n1, n2)));
}

static void fromStr (Machine *m) {
  machine_push(m, token_new_int(
    js_rl((Js *)tk_pop_string(m))
  ));
}

static void sabs (Machine *m) {
  Int i = tk_pop_int(m);
  machine_push(m, token_new_int(i >= 0 ? i : -i));
}

static void rnd (Machine *m) {
  machine_push(m, token_new_int(
    rnd_i(tk_pop_int(m))
  ));
}

static void sdiv (Machine *m) {
  Int den = tk_pop_int(m);
  Int num = tk_pop_int(m);
  Int quot = num / den;
  Int rem = num - quot * den;
  machine_push(m, token_new_int(quot));
  machine_push(m, token_new_int(rem));
}

static void and (Machine *m) {
  Int i2 = tk_pop_int(m);
  Int i1 = tk_pop_int(m);
  machine_push(m, token_new_int(i1 & i2));
}

static void or (Machine *m) {
  Int i2 = tk_pop_int(m);
  Int i1 = tk_pop_int(m);
  machine_push(m, token_new_int(i1 | i2));
}

static void xor (Machine *m) {
  Int i2 = tk_pop_int(m);
  Int i1 = tk_pop_int(m);
  machine_push(m, token_new_int(i1 ^ i2));
}

static void not (Machine *m) {
  Int i = tk_pop_int(m);
  machine_push(m, token_new_int(~i));
}

static void left (Machine *m) {
  Int i2 = tk_pop_int(m);
  Int i1 = tk_pop_int(m);
  machine_push(m, token_new_int(i1 << i2));
}

static void right (Machine *m) {
  Int i2 = tk_pop_int(m);
  Int i1 = tk_pop_int(m);
  machine_push(m, token_new_int(i1 >> i2));
}

static void max (Machine *m) {
  int mx (int n1, int n2) { return n1 >= n2 ? n1 : n2; }
  fn2(m, mx);
}

static void min (Machine *m) {
  int mn (int n1, int n2) { return n1 <= n2 ? n1 : n2; }
  fn2(m, mn);
}

static void maxint (Machine *m) {
  Int r = 0;
  if (sizeof(Int) ==  sizeof(int)) r = INT_MAX;
  else if (sizeof(Int) ==  sizeof(long int)) r = LONG_MAX;
  else if (sizeof(Int) ==  sizeof(long long int)) r = LLONG_MAX;
  machine_push(m, token_new_int(r));
}

static void minint (Machine *m) {
  Int r = 0;
  if (sizeof(Int) ==  sizeof(int)) r = INT_MIN;
  else if (sizeof(Int) ==  sizeof(long int)) r = LONG_MIN;
  else if (sizeof(Int) ==  sizeof(long long int)) r = LLONG_MIN;
  machine_push(m, token_new_int(r));
}

static void tofloat (Machine *m) {
  machine_push(m, token_new_float(
    (double)tk_pop_int(m)
  ));
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
  add("&", and);
  add("|", or);
  add("^", xor);
  add("~", not);
  add("<<", left);
  add(">>", right);

  add("max", max); // [INT, INT] - INT
  add("min", min); // [INT, INT] - INT

  add("maxInt", maxint); // () - INT
  add("minInt", minint); // () - INT
  add("toFloat", tofloat); // INT - FLOAT
  return r;
}


