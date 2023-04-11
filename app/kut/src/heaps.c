// Copyright 29-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heaps.h"
#include "kut/DEFS.h"

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

int heaps_add_symbol (Heaps *this, char *symbol, Exp *exp) {
  return heap_add(this->hp, symbol, exp);
}

int heaps_is_duplicate (Heaps *this, char *symbol) {
  return opt_get(map_get(heap_get(this->hp), symbol)) ? TRUE : FALSE;
}

// <Opt<Exp>, Heap>
Tp *heaps_get (Heaps *this, char *symbol) {
  for (;;) {
    Opt *exp = map_get(heap_get(this->hp), symbol);
    if (opt_get(exp)) return tp_new(exp, NULL);
    Heaps *new = this->hps;
    if (new == &root) break;
    this = new;
  }
  return tp_new(opt_none(), this->hp);
}
