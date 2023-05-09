// Copyright 12-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "imports.h"

struct imports_ImportsEntry {
  int symbol;
  int fix;
};
static ImportsEntry *imports_entry_new (int symbol, int fix) {
  ImportsEntry *this = MALLOC(ImportsEntry);
  this->symbol = symbol;
  this->fix = fix;
  return this;
}

int imports_entry_symbol (ImportsEntry *this) {
  return this->symbol;
}

int imports_entry_fix (ImportsEntry *this) {
  return this->fix;
}

Imports *imports_new (void) {
  return (Imports *) arr_new();
}

void imports_add(Imports *this, int symbol, int fix) {
  arr_push((Arr *)this, imports_entry_new(symbol, fix));
}

int imports_get_fix (Imports *this, int symbol) {
  EACH(this, ImportsEntry, e) {
    if (e->symbol == symbol) return e->fix;
  }_EACH
  return -1;
}

// <ImportsEntry>
Arr *imports_get_array (Imports *this) {
  return (Arr *) this;
}
