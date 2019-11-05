// Copyright 02-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types.h"
#include "tk.h"

// stack is Arr<Token>
static char *check (Arr *stack, char *type) {
  if (!*type) {
    int sz = arr_size(stack);
    if (sz)
      return str_f(
        "%d value%s in stack",
        sz, sz == 1 ? "" : "s"
      );
    else
      return "";
  }
  return token_check_type(list_reverse(list_from_arr(stack)), type);
}

void types_fail (Machine *m) {
  char *t = symbol_to_str(token_symbol(*arr_start(tk_pop_list(m))));
  char *checked = check(machine_stack(m), t);
  if (!str_eq(checked, t))
    machine_fail(m, str_f(
      "Stack type check failed. Expected: '@%s'. Actual: '@%s'", t, checked
    ));
}

void types_check (Machine *m) {
  char *t = symbol_to_str(token_symbol(*arr_start(tk_pop_list(m))));
  char *checked = check(machine_stack(m), t);
  machine_push(m, token_new_int(str_eq(t, checked)));
}
