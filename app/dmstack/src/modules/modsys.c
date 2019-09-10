// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modsys.h"
#include "fails.h"
#include "modules/modsys0.h"
#include "modules/modsys1.h"
#include "modules/modsys2.h"
#include "Token.h"

static void print (Machine *m) {
  puts(token_to_str(machine_pop(m)));
}

static void to_str (Machine *m) {
  machine_push(m, token_new_string(token_to_str(machine_pop(m))));
}

static void clone (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_clone(tk));
}

static void eq (Machine *m) {
  Token *tk2 = machine_pop(m);
  Token *tk1 = machine_pop(m);
  machine_push(m, token_new_int(token_eq(tk1, tk2)));
}

static void neq (Machine *m) {
  Token *tk2 = machine_pop(m);
  Token *tk1 = machine_pop(m);
  machine_push(m, token_new_int(!token_eq(tk1, tk2)));
}

static void fail (Machine *m) {
  Token *tk = machine_pop_exc(m, token_STRING);
  machine_fail(m, token_string(tk));
}

static void sleep (Machine *m) {
  Token *tk = machine_pop_exc(m, token_INT);
  sys_sleep(token_int(tk));
}

// Resturns Map<primitives_Fn>
Map *modsys_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "print", print);
  map_put(r, "toStr", to_str);
  map_put(r, "clone", clone);
  map_put(r, "==", eq);
  map_put(r, "!=", neq);
  map_put(r, "fail", fail);
  map_put(r, "sleep", sleep);

  // modsys0 -------------------------------------
  map_put(r, "+", modsys0_add);
  map_put(r, "-", modsys0_sub);
  map_put(r, "*", modsys0_mul);
  map_put(r, "/", modsys0_div);
  map_put(r, "%", modsys0_mod);
  map_put(r, "++", modsys0_incr);
  map_put(r, "--", modsys0_decr);

  // modsys1 -------------------------------------
  map_put(r, "&&", modsys1_and);
  map_put(r, "||", modsys1_or);
  map_put(r, "!", modsys1_not);
  map_put(r, ">", modsys1_greater);
  map_put(r, ">=", modsys1_greater_eq);
  map_put(r, "<", modsys1_less);
  map_put(r, "<=", modsys1_less_eq);

  // modsys2 -------------------------------------
  map_put(r, "size", modsys2_size);
  map_put(r, "get", modsys2_get);
  map_put(r, "set", modsys2_set);
  map_put(r, "sub", modsys2_sub);
  map_put(r, "left", modsys2_left);
  map_put(r, "right", modsys2_right);

  return r;
}
