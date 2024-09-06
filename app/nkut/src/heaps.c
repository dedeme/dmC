// Copyright 29-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heaps.h"
#include "kut/DEFS.h"

struct heaps_Heaps {
  Heap *hp;
  Heaps *hps;
};

static Heaps root = { .hp = NULL, .hps = NULL };

Heaps *heaps_new (void) {
  return &root;
}

Heaps *heaps_add (Heaps *this, Heap *hp) {
  Heaps *r = MALLOC(Heaps);
  r->hp = hp;
  r->hps = this;
  return r;
}

int heaps_add_symbol (Heaps *this, char *symbol) {
  return heap_add(this->hp, symbol);
}

int heaps_is_duplicate (Heaps *this, char *symbol) {
  return heap_contains(this->hp, symbol) ? TRUE : FALSE;
}

int heaps_contains (Heaps *this, char *symbol) {
  for (;;) {
    if (this == &root) break;
    if (heap_contains(this->hp, symbol)) return TRUE;
    this = this->hps;
  }
  return FALSE;
}
