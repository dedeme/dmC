// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap0.h"
#include "kut/DEFS.h"
#include "kut/map.h"

struct heap0_Heap0Entry {
  int nline;
  Exp *exp;
};

static Heap0Entry *new_entry (int nline, Exp *exp) {
  Heap0Entry *this = MALLOC(Heap0Entry);
  this->nline = nline;
  this->exp = exp;
  return this;
}

int heap0_entry_nline (Heap0Entry *entry) {
  return entry->nline;
}

Exp *heap0_entry_exp (Heap0Entry *entry) {
  return entry->exp;
}

Heap0 *heap0_new(void) {
  return (Heap0 *)map_new();
}

/// Returns a Map<Heap0Entry>.
Map *heap0_get(Heap0 *this) {
  return (Map *) this;
}

int heap0_add(Heap0 *this, char *symbol, int nline, Exp *exp) {
  if (map_has_key((Map *)this, symbol)) return FALSE;
  map_add((Map *)this, symbol, new_entry(nline, exp));
  return TRUE;
}


