// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modglobal.h"
#include "fails.h"
#include "primitives/modglobal0.h"
#include "primitives/modglobal1.h"
#include "primitives/modglobal2.h"
#include "Token.h"

static void sputs (Machine *m) {
  puts(token_to_str(machine_pop(m)));
}

static void to_str (Machine *m) {
  machine_push(m, token_new_string(0, token_to_str(machine_pop(m))));
}

static void clone (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_clone(tk));
}

static void eq (Machine *m) {
  Token *tk2 = machine_pop(m);
  Token *tk1 = machine_pop(m);
  machine_push(m, token_new_int(0, token_eq(tk1, tk2)));
}

static void neq (Machine *m) {
  Token *tk2 = machine_pop(m);
  Token *tk1 = machine_pop(m);
  machine_push(m, token_new_int(0, !token_eq(tk1, tk2)));
}

static void fail (Machine *m) {
  Token *tk = machine_pop_exc(m, token_STRING);
  machine_fail(m, token_string(tk));
}

static void stkfail (Machine *m, char *fn, int min) {
  machine_fail(m, str_f(
    "'%s' require at less %d elements in stack. Current number is %d.",
    fn, min, arr_size(machine_stack(m))
  ));
}

static void swap (Machine *m) {
  Arr *stack = machine_stack(m);
  int sz = arr_size(stack);
  if (sz > 1) {
    Token *tk = arr_get(stack, --sz);
    arr_set(stack, sz, arr_get(stack, sz - 1));
    arr_set(stack, sz - 1, tk);
    return;
  }
  stkfail(m, "swap", 2);
}

static void pop (Machine *m) {
  Arr *stack = machine_stack(m);
  if (arr_size(stack)) {
    arr_pop(stack);
    return;
  }
  stkfail(m, "pop", 1);
}

static void dup (Machine *m) {
  Arr *stack = machine_stack(m);
  if (arr_size(stack)) {
    arr_push(stack, arr_peek(stack));
    return;
  }
  stkfail(m, "dup", 1);
}

static void empty (Machine *m) {
  Arr *stack = machine_stack(m);
  arr_push(stack, token_new_int(0, arr_size(stack) == 0));
}

// Resturns Map<primitives_Fn>
Map *modglobal_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "puts", sputs);
  map_put(r, "toStr", to_str);
  map_put(r, "clone", clone);
  map_put(r, "==", eq);
  map_put(r, "!=", neq);
  map_put(r, "fail", fail);
  map_put(r, "swap", swap);
  map_put(r, "pop", pop);
  map_put(r, "dup", dup);
  map_put(r, "empty?", empty);

  // modglobal0 -------------------------------------
  map_put(r, "+", modglobal0_add);
  map_put(r, "-", modglobal0_sub);
  map_put(r, "*", modglobal0_mul);
  map_put(r, "/", modglobal0_div);
  map_put(r, "%", modglobal0_mod);
  map_put(r, "++", modglobal0_incr);
  map_put(r, "--", modglobal0_decr);

  // modglobal1 -------------------------------------
  map_put(r, "&&", modglobal1_and);
  map_put(r, "||", modglobal1_or);
  map_put(r, "!", modglobal1_not);
  map_put(r, ">", modglobal1_greater);
  map_put(r, ">=", modglobal1_greater_eq);
  map_put(r, "<", modglobal1_less);
  map_put(r, "<=", modglobal1_less_eq);

  // modglobal2 -------------------------------------
  map_put(r, "size", modglobal2_size);
  map_put(r, "get", modglobal2_get);
  map_put(r, "set", modglobal2_set);

  return r;
}
