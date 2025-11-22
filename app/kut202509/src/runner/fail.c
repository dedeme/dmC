// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "runner/fail.h"
#include "stat.h"
#include "fileix.h"

char *fail_add_stack (char *msg, Stack *stack) {
  // <char>
  Arr *r = stack_to_arr(stack);
  arr_insert(r, 0, msg);
  return arr_join(r, "\n  ");
}

char *fail_type (char *expected, Exp *exp) {
  char *sexp = exp_to_js(exp);
  // char
  Arr *runes = str_runes(sexp);
  int size = arr_size(runes);
  if (size > 160)
    sexp = str_f("...%s", arr_join(arr_drop(runes, size - 157), ""));

  return str_f(
    "Type error:\n    Expected: %s\n    Found   : %s (%s)",
    expected,
    exp_type_to_str(exp),
    sexp
  );
}

char *fail_bfunction (char *mod, char *func) {
  return str_f("Function '%s' not found in module '%s'", func, mod);
}
