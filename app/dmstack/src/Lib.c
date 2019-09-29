// Copyright 16-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Lib.h"

struct lib_Entry {
  Symbol s;
  Heap *heap;
};
static LibEntry *entry_new (Symbol s, Heap *heap) {
  LibEntry *this = MALLOC(LibEntry);
  this->s = s;
  this->heap = heap;
  return this;
}

Symbol libEntry_symbol (LibEntry *this) {
  return this->s;
}

Heap *libEntry_heap (LibEntry *this) {
  return this->heap;
}

Lib *lib_new (void) {
  return (Lib *)arr_new();
}

void lib_add (Lib *this, Symbol s, Heap *heap) {
  EACH((Arr *)this, LibEntry, e) {
    if (symbol_eq(e->s, s)) return;
  }_EACH
  arr_push((Arr *)this, entry_new(s, heap));
}

Heap *lib_get (Lib *this, Symbol s) {
  EACH((Arr *)this, LibEntry, e) {
    if (symbol_eq(e->s, s)) return e->heap;
  }_EACH
  return NULL;
}

// Arr<LibEntry>
Arr *lib_entries (Lib *this) {
  return (Arr *)this;
}

