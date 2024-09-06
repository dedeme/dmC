// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap.h"
#include "kut/DEFS.h"
#include "kut/map.h"

Heap *heap_new (void) {
  return (Heap *)arr_new();
}

// <char>
Arr *heap_get_array (Heap *this) {
  return (Arr *)this;
}

int heap_add (Heap *this, char *symbol) {
  EACH(this, char, e) {
    if (!strcmp(e, symbol)) return FALSE;
  }_EACH
  arr_push((Arr *)this, symbol);
  return TRUE;
}

int heap_contains(Heap *this, char *sym) {
  EACH(this, char, e) {
    if (!strcmp(e, sym)) return TRUE;
  }_EACH
  return FALSE;
}
