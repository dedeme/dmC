// Copyright 11-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "modules.h"
#include "kut/arr.h"

// Arr<Opt<Opt<Module>>>
static Arr *a;

void modules_init (void) {
  a = arr_new();
}

void modules_add (int fix) {
  int inc = fix + 1 - arr_size(a);
  if (inc > 0) {
    REPEAT(inc) {
      arr_push(a, opt_none());
    }_REPEAT
  } else if (opt_get(arr_get(a, fix))) {
    return;
  }

  arr_set(a, fix, opt_some(opt_none()));
}

void modules_set (int ix, Module *mod) {
  arr_set(a, ix, opt_some(opt_some(mod)));
}

Opt *modules_get(int ix) {
  if (ix < arr_size(a))
    return arr_get(a, ix);
  return opt_none();
}

Opt *modules_get_ok(int ix) {
  if (ix < arr_size(a)) {
    Opt *r = opt_get(arr_get(a, ix));
    if (r) return r;
  }
  EXC_IO(str_f("Module '%d' not found", ix));
  return NULL;
}

Arr *modules_get_array(void) {
  return arr_copy(a);
}
