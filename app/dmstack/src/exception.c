// Copyright 29-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "exception.h"

void exception_throw(Machine *m, char *type, char *msg) {
  Js *msg_stack = arr_to_js(
    arr_new_from(type, msg, machine_stack_trace(m), NULL),
    (FTO) js_ws
  );
  THROW(exc_dmstack_t) (char *)msg_stack _THROW
}

char *exception_type(Exc *ex) {
  if (str_eq(exc_type(ex), exc_dmstack_t)) {
    char *m = exc_msg(ex);
    m = str_right(m, str_cindex(m, '\n') + 1);
    return arr_get(arr_from_js((Js *)m, (FFROM) js_rs), 0);
  }
  return exc_type(ex);
}

char *exception_msg(Exc *ex) {
  if (str_eq(exc_type(ex), exc_dmstack_t)) {
    char *m = exc_msg(ex);
    m = str_right(m, str_cindex(m, '\n') + 1);
    return arr_get(arr_from_js((Js *)m, (FFROM) js_rs), 1);
  }
  return exc_msg(ex);
}

char *exception_stack(Exc *ex) {
  if (str_eq(exc_type(ex), exc_dmstack_t)) {
    char *m = exc_msg(ex);
    m = str_right(m, str_cindex(m, '\n') + 1);
    return arr_get(arr_from_js((Js *)m, (FFROM) js_rs), 2);
  }
  return str_cjoin(exc_stack(ex), '\n');
}
