// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap.h"
#include "kut/DEFS.h"
#include "kut/map.h"

Heap *heap_new (void) {
  return (Heap *)map_new();
}

/// Returns a Map<Exp> wose keys are the defined symbols.
Map *heap_get (Heap *this) {
  return (Map *)this;
}

int heap_add (Heap *this, char *symbol, Exp *exp) {
  if (map_has_key((Map *)this, symbol)) return FALSE;
  map_add((Map *)this, symbol, exp);
  return TRUE;
}

Opt *heap_get_exp (Arr *heaps, char *symbol) {
  EACH(heaps, Heap, h) {
    Opt *e = map_get((Map *)h, symbol);
    if (opt_get(e)) return e;
  }_EACH
  return opt_none();
}

Arr *heap_add_to_arr (Heap *this, Arr *heaps) {
  Arr *r = arr_copy(heaps);
  arr_insert(r, 0, this);
  return r;
}
