// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "modules.h"
#include "kut/arr.h"

// Opt<Module>
static Arr *a;

void modules_init (void) {
  a = arr_new();
}

void modules_add (int ix, Module *mod) {
  int inc = ix + 1 - arr_size(a);
  if (inc > 0) {
    REPEAT(inc) {
      arr_push(a, opt_none());
    }_REPEAT
  }
  arr_set(a, ix, opt_some(mod));
}

// Module
Opt *modules_get(int ix) {
  return arr_get(a, ix);
}

// Opt<Module>
Arr *modules_get_array() {
  return a;
}
