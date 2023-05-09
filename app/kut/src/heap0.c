// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "heap0.h"
#include "kut/map.h"

struct heap0_Heap0Entry {
  int symbol;
  int nline;
  Exp *exp;
};

static Heap0Entry *new_entry (int symbol, int nline, Exp *exp) {
  Heap0Entry *this = MALLOC(Heap0Entry);
  this->symbol = symbol;
  this->nline = nline;
  this->exp = exp;
  return this;
}

int heap0_entry_symbol (Heap0Entry *entry) {
  return entry->symbol;
}

int heap0_entry_nline (Heap0Entry *entry) {
  return entry->nline;
}

Exp *heap0_entry_exp (Heap0Entry *entry) {
  return entry->exp;
}

Heap0 *heap0_new(void) {
  return (Heap0 *)arr_new();
}

/// Returns an Arr<Heap0Entry>.
Arr *heap0_get_array(Heap0 *this) {
  return (Arr *) this;
}

int heap0_add(Heap0 *this, int symbol, int nline, Exp *exp) {
  EACH(this, Heap0Entry, e) {
    if (e->symbol == symbol) return FALSE;
  }_EACH
  arr_push((Arr *)this, new_entry(symbol, nline, exp));
  return TRUE;
}

// <Heap0Entry>
Opt *heap0_get(Heap0 *this, int sym) {
  EACH(this, Heap0Entry, e) {
    if (e->symbol == sym) return opt_some(e);
  }_EACH
  return opt_none();
}
