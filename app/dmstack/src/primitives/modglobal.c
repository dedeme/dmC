// Copyright 28-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modglobal.h"
#include "fails.h"
#include "primitives/modglobal0.h"
#include "primitives/modglobal1.h"
#include "primitives/modglobal2.h"
#include "Token.h"
#include "tk.h"
#include "exception.h"

static void sputs (Machine *m) {
  puts(token_to_str_draft(machine_pop(m)));
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
  exception_throw(m, "generic", tk_pop_string(m));
}

static void throw (Machine *m) {
  char *msg = tk_pop_string(m);
  exception_throw(m, tk_pop_string(m), msg);
}

static void try (Machine *m) {
  // Arr<Token>
  Arr *a = arr_copy(tk_pop_list(m));
  Token *prgtry = machine_pop_exc(m, token_LIST);
  TRY {
    machine_isolate_process("", machine_pmachines(m), prgtry);
  } CATCH (ex) {
    arr_insert(a, 0, token_from_pointer(symbol_EXC_, ex));
    machine_isolate_process("", machine_pmachines(m), token_new_list(a));
  }_TRY
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
  arr_push(stack, token_new_int(arr_size(stack) == 0));
}

Pmodule *modglobal_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("puts", sputs);
  add("toStr", to_str);
  add("clone", clone);
  add("==", eq);
  add("!=", neq);
  add("fail", fail);
  add("throw", throw);
  add("try", try);
  add("swap", swap);
  add("pop", pop);
  add("dup", dup);
  add("empty?", empty);

  // modglobal0 -------------------------------------
  add("+", modglobal0_add);
  add("-", modglobal0_sub);
  add("*", modglobal0_mul);
  add("/", modglobal0_div);
  add("%", modglobal0_mod);
  add("++", modglobal0_incr);
  add("--", modglobal0_decr);

  // modglobal1 -------------------------------------
  add("&&", modglobal1_and);
  add("||", modglobal1_or);
  add("!", modglobal1_not);
  add(">", modglobal1_greater);
  add(">=", modglobal1_greater_eq);
  add("<", modglobal1_less);
  add("<=", modglobal1_less_eq);

  // modglobal2 -------------------------------------
  add(">>", modglobal2_get);
  add("<<", modglobal2_set);
  add("<<+", modglobal2_setplus);
  add("^^", modglobal2_up);
  add("^^+", modglobal2_upplus);

  return r;
}
