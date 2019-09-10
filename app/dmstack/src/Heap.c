// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Heap.h"

struct heap_Entry {
  Symbol *s;
  Token *tk;
};
static HeapEntry *entry_new (Symbol *s, Token *tk) {
  HeapEntry *this = MALLOC(HeapEntry);
  this->s = s;
  this->tk = tk;
  return this;
}

Symbol *heapEntry_symbol (HeapEntry *this) {
  return this->s;
}

Token *heapEntry_token (HeapEntry *this) {
  return this->tk;
}

Heap *heap_new (void) {
  return (Heap *)arr_new();
}

void heap_add (Heap *this, Symbol *s, Token *tk) {
  arr_push((Arr *)this, entry_new(s, tk));
}

Token *heap_get (Heap *this, Symbol *s) {
  EACH((Arr *)this, HeapEntry, e) {
    if (symbol_eq(e->s, s)) return e->tk;
  }_EACH
  return NULL;
}

int heap_set (Heap *this, Symbol *s, Token *tk) {
  EACH((Arr *)this, HeapEntry, e) {
    if (symbol_eq(e->s, s)) {
      e->tk = tk;
      return 1;
    }
  }_EACH
  return 0;
}

void heap_remove (Heap *this, Symbol *s) {
  int i = -1;
  EACH_IX((Arr *)this, HeapEntry, e, ix) {
    if (symbol_eq(e->s, s)) {
      i = ix;
      break;
    }
  }_EACH
  if (i > -1) arr_remove((Arr *)this, i);
}

Arr *heap_entries (Heap *this) {
  return (Arr *)this;
}
