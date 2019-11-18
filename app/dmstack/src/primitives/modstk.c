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
  printf("  [ ");
  int sz1 = arr_size(st) - 1;
  if (sz1 >= 0) {
    printf("%s", token_to_str_draft(arr_get(st, sz1)));
    for(int i = sz1 - 1; i >= 0; --i)
      printf(", %s", token_to_str(arr_get(st, i)));
  }
  puts("]");
}

static void size (Machine *m) {
  machine_push(m, token_new_int(arr_size(machine_stack(m))));
}

Pmodule *modstk_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("clear", clear);
  add("show", show);
  add("size", size);

  return r;
}

