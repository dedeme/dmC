// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "heap.h"
#include "kut/map.h"

struct heap_HeapEntry {
  int symbol;
  Exp *exp;
};

static HeapEntry *new_entry (int symbol, Exp *exp) {
  HeapEntry *this = MALLOC(HeapEntry);
  this->symbol = symbol;
  this->exp = exp;
  return this;
}

int heap_entry_symbol (HeapEntry *entry) {
  return entry->symbol;
}

Exp *heap_entry_exp (HeapEntry *entry) {
  return entry->exp;
}

Heap *heap_new (void) {
  return (Heap *)arr_new();
}

// <MapEntry>
Arr *heap_get_array (Heap *this) {
  return (Arr *)this;
}

int heap_add (Heap *this, int symbol, Exp *exp) {
  EACH(this, HeapEntry, e) {
    if (e->symbol == symbol) return FALSE;
  }_EACH
  arr_push((Arr *)this, new_entry(symbol, exp));
  return TRUE;
}

// <HeapEntry>
Opt *heap_get(Heap *this, int sym) {
  EACH(this, HeapEntry, e) {
    if (e->symbol == sym) return opt_some(e);
  }_EACH
  return opt_none();
}
