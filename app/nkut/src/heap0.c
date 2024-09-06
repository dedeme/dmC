// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "heap0.h"
#include "kut/DEFS.h"
#include "kut/map.h"

struct heap0_Heap0Entry {
  char *symbol;
  int nline;
};

static Heap0Entry *new_entry (char *symbol, int nline) {
  Heap0Entry *this = MALLOC(Heap0Entry);
  this->symbol = symbol;
  this->nline = nline;
  return this;
}

char *heap0_entry_symbol (Heap0Entry *entry) {
  return entry->symbol;
}

int heap0_entry_nline (Heap0Entry *entry) {
  return entry->nline;
}

Heap0 *heap0_new(void) {
  return (Heap0 *)arr_new();
}

/// Returns an Arr<Heap0Entry>.
Arr *heap0_get_array(Heap0 *this) {
  return (Arr *) this;
}

int heap0_add(Heap0 *this, char *symbol, int nline) {
  EACH(this, Heap0Entry, e) {
    if (!strcmp(e->symbol, symbol)) return FALSE;
  }_EACH
  arr_push((Arr *)this, new_entry(symbol, nline));
  return TRUE;
}

// <Heap0Entry>
Opt *heap0_get(Heap0 *this, char *sym) {
  EACH(this, Heap0Entry, e) {
    if (!strcmp(e->symbol, sym)) return opt_some(e);
  }_EACH
  return opt_none();
}
