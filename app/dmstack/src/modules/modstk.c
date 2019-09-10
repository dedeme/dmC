// Copyright 02-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modstk.h"
#include "fails.h"
#include "Machine.h"

static void stkfail (Machine *m, int min) {
  machine_fail(m, str_f(
    "swap require at less %d elements in stack. Current number is %d.",
    min, arr_size(machine_stack(m))
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
  stkfail(m, 2);
}

static void pop (Machine *m) {
  Arr *stack = machine_stack(m);
  if (arr_size(stack)) {
    arr_pop(stack);
    return;
  }
  stkfail(m, 1);
}

static void clear (Machine *m) {
  arr_clear(machine_stack(m));
}

static void dup (Machine *m) {
  Arr *stack = machine_stack(m);
  if (arr_size(stack)) {
    arr_push(stack, arr_peek(stack));
    return;
  }
  stkfail(m, 1);
}

static void empty (Machine *m) {
  Arr *stack = machine_stack(m);
  arr_push(stack, token_new_int(arr_size(stack) == 0));
}

static void show (Machine *m) {
  Arr *st = machine_stack(m);
  puts("Stack:");
  if (!arr_size(st)) {
    puts("  [EMPTY]");
    return;
  }
  printf("  [");
  while (arr_size(st))
    printf("%s ", token_to_str(arr_pop(st)));
  puts("]");
}

// Resturns Map<primitives_Fn>
Map *modstk_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "stk.swap", swap);
  map_put(r, "stk.pop", pop);
  map_put(r, "stk.clear", clear);
  map_put(r, "stk.dup", dup);
  map_put(r, "stk.empty?", empty);
  map_put(r, "stk.show", show);

  return r;
}

