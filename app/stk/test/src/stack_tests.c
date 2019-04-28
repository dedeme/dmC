// Copyright 18-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "stack_tests.h"
#include "assert.h"
#include "Stack.h"

void stack_tests(void) {
  puts("Stack tests:");

  Stack *st = stack_new();
  assert(stack_empty(st));
  assert(stack_count(st) == 0);
  assert(!stack_has_element(st, 0));
  assert(!stack_has_element(st, 200));
  stack_free(st, free);

  st = stack_new();
  stack_push(st, str_new("a"));
  assert(!stack_empty(st));
  assert(stack_count(st) == 1);
  assert(stack_has_element(st, 0));
  assert(!stack_has_element(st, 200));
  assert(str_eq("a", stack_peek(st)));
  assert(str_eq("a", stack_peek_n(st, 0)));

  stack_clear(st, free);
  assert(stack_empty(st));
  assert(!stack_has_element(st, 0));

  stack_push(st, str_new("a"));
  assert(!stack_empty(st));
  stack_del(st, 0, free);
  assert(stack_empty(st));
  assert(!stack_has_element(st, 0));

  stack_push(st, str_new("a"));
  assert(!stack_empty(st));
  stack_clear_n(st, 2, free);
  assert(stack_empty(st));
  assert(!stack_has_element(st, 0));

  stack_push(st, str_new("a"));
  stack_push(st, str_new("b"));
  stack_push(st, str_new("c"));
  stack_push(st, str_new("d"));
  stack_push(st, str_new("e"));
  assert(!stack_empty(st));
  assert(stack_count(st) == 5);
  assert(stack_has_element(st, 0));
  assert(stack_has_element(st, 4));
  assert(!stack_has_element(st, 200));
  assert(!stack_has_element(st, 5));
  assert(str_eq("e", stack_peek(st)));

  char *v = stack_pop_new(st);
  free(v);
  assert(!stack_empty(st));
  assert(stack_count(st) == 4);
  assert(stack_has_element(st, 3));
  assert(!stack_has_element(st, 4));
  assert(str_eq("d", stack_peek(st)));

  stack_del(st, 1, free);
  assert(!stack_empty(st));
  assert(stack_count(st) == 3);
  assert(stack_has_element(st, 2));
  assert(!stack_has_element(st, 3));
  assert(str_eq("d", stack_peek(st)));
  assert(str_eq("a", stack_peek_n(st, 2)));

  stack_push(st, str_new("e"));
  stack_del_range(st, 1, 3, free);
  assert(!stack_empty(st));
  assert(stack_count(st) == 2);
  assert(stack_has_element(st, 1));
  assert(!stack_has_element(st, 2));
  assert(str_eq("e", stack_peek(st)));
  assert(str_eq("a", stack_peek_n(st, 1)));

  v = stack_remove_new(st, 0);
  free(v);
  assert(!stack_empty(st));
  assert(stack_count(st) == 1);
  assert(str_eq("a", stack_peek_n(st, 0)));

  stack_push(st, str_new("b"));
  stack_push(st, str_new("c"));
  stack_push(st, str_new("d"));
  stack_push(st, str_new("e"));
  assert(!stack_empty(st));
  assert(stack_count(st) == 5);
  assert(stack_has_element(st, 0));
  assert(stack_has_element(st, 4));
  assert(!stack_has_element(st, 200));
  assert(!stack_has_element(st, 5));
  assert(str_eq("e", stack_peek(st)));

  stack_del_range(st, 1, 3, free);
  assert(!stack_empty(st));
  assert(stack_count(st) == 3);
  assert(stack_has_element(st, 0));
  assert(stack_has_element(st, 2));
  assert(!stack_has_element(st, 3));
  assert(str_eq("e", stack_peek(st)));
  assert(str_eq("a", stack_peek_n(st, 2)));

  stack_del_range(st, 1, 2, free);
  assert(stack_count(st) == 2);
  assert(str_eq("e", stack_peek(st)));
  assert(str_eq("a", stack_peek_n(st, 1)));

  Arr *a = stack_remove_range_new(st, 0, 2, free);
  assert(stack_empty(st));
  assert(arr_size(a) == 2);
  assert(str_eq(arr_get(a, 0), "e"));
  assert(str_eq(arr_get(a, 1), "a"));
  arr_free(a);

  stack_free(st, free);

  puts("    Finished");
}


