// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "runner/fail.h"
#include "stat.h"
#include "kut/DEFS.h"
#include "fileix.h"

char *fail_add_stack (char *msg, Stack *stack) {
  // <char>
  Arr *r = stack_to_arr(stack);
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
