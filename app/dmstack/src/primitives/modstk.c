// Copyright 02-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modstk.h"
#include "fails.h"
#include "Machine.h"

static void clear (Machine *m) {
  arr_clear(machine_stack(m));
}

static void show (Machine *m) {
  Arr *st = machine_stack(m);
  puts("Stack:");
  if (!arr_size(st)) {
    puts("  [EMPTY]");
    return;
  }
  printf("  [ ");
  for(int i = arr_size(st) - 1; i >= 0; --i)
    printf("%s ", token_to_str(arr_get(st, i)));
  puts("]");
}

Pmodule *modstk_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("clear", clear);
  add("show", show);

  return r;
}

