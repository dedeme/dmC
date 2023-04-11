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

