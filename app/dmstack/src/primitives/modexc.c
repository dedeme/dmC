// Copyright 29-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modexc.h"
#include "tk.h"
#include "exception.h"

static void type (Machine *m) {
  machine_push(m, token_new_string(
    exception_type(tk_pop_native(m, symbol_EXC_))
  ));
}

static void msg (Machine *m) {
  machine_push(m, token_new_string(
    exception_msg(tk_pop_native(m, symbol_EXC_))
  ));
}

static void stack (Machine *m) {
  machine_push(m, token_new_string(
    exception_stack(tk_pop_native(m, symbol_EXC_))
  ));
}

Pmodule *modexc_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("type", type); // Exc - STRING
  add("msg", msg); // Exc - STRING
  add("stack", stack); // Exc - STRING

  return r;
}


