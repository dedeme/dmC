// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Symbol.h"

struct symbol_Symbol {
  char *name;
  int hash;
};

Symbol *symbol_new (char *name) {
  Symbol *this = MALLOC(Symbol);
  this->name = name;
  int hash = 0;
  int base = 0;
  char *p = name;
  while (*p) {
    hash += base + *p++;
    base += 75;
  }
  this->hash = hash;
  return this;
}

char *symbol_name (Symbol *this) {
  return this->name;
}

int symbol_hash (Symbol *this) {
  return this->hash;
}

Symbol *symbol_clone (Symbol *this) {
  Symbol *r = MALLOC(Symbol);
  r->name = this->name;
  r->hash = this->hash;
  return r;
}

int symbol_eq (Symbol *this, Symbol *other) {
  return this->hash == other->hash && str_eq(this->name, other->name);
}

char *symbol_to_str (Symbol *this) {
  return this->name;
}
