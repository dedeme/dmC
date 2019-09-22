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
  printf("  [");
  while (arr_size(st))
    printf("%s ", token_to_str(arr_pop(st)));
  puts("]");
}

// Resturns Map<primitives_Fn>
Map *modstk_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "clear", clear);
  map_put(r, "show", show);

  return r;
}

