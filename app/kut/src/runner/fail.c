// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "runner/fail.h"
#include "stat.h"
#include "kut/DEFS.h"
#include "fileix.h"

// Returns Arr<char>. stack is Arr<Stat>
Arr *fail_stack_to_str (Arr *stack) {
  Arr *r = arr_new();
  int ix = arr_size(stack) - 1;
  for (int i = 0; i < 15; ++i, --ix) {
    if (ix < 0) break;

    StatCode *st = arr_get(stack, ix);
    arr_push(r, str_f(
      "%s:%d:",
      fileix_to_fail(stat_code_file_ix(st)),
      stat_code_line(st)
    ));
  }
  return r;
}

char *fail_add_stack (char *msg, Arr *stack) {
  // <char>
  Arr *r = fail_stack_to_str(stack);
  arr_insert(r, 0, msg);
  return arr_join(r, "\n  ");
}

char *fail_type (char *expected, Exp *exp) {
  return str_f(
    "Type error:\n    Expected: %s\n    Found   : %s (%s)",
    expected,
    exp_type_to_str(exp),
    exp_to_js(exp)
  );
}

char *fail_bfunction (char *mod, char *func) {
  return str_f("Function '%s' not found in module '%s'", func, mod);
}
