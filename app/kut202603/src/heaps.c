// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "heaps.h"

struct heaps_Heaps {
  Heap *hp;
  Heaps *hps;
};

static Heaps root = { .hp = NULL, .hps = NULL };

Heaps *heaps_new (Heap *hp) {
  return heaps_add(&root, hp);
}

Heaps *heaps_add (Heaps *this, Heap *hp) {
  Heaps *r = MALLOC(Heaps);
  r->hp = hp;
  r->hps = this;
  return r;
}

int heaps_add_symbol (Heaps *this, int symbol, Exp *exp) {
  return heap_add(this->hp, symbol, exp);
}

int heaps_is_duplicate (Heaps *this, int symbol) {
  return opt_get(heap_get(this->hp, symbol)) ? TRUE : FALSE;
}

int heaps_is_initial (Heaps *this) {
  return this->hps == &root;
}

// <Opt<Exp>, Heap>
Tp *heaps_get (Heaps *this, int symbol) {
  for (;;) {
    HeapEntry *entry = opt_get(heap_get(this->hp, symbol));
    if (entry) return tp_new(opt_some(heap_entry_exp(entry)), NULL);
    Heaps *new = this->hps;
    if (new == &root) break;
    this = new;
  }
  return tp_new(opt_none(), this->hp);
}
