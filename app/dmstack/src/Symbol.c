// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Symbol.h"

struct symbol_Symbol {
  char *name;
  char *id;
  int hash;
};

Symbol *symbol_new_id (char *id, char *name) {
  Symbol *this = MALLOC(Symbol);
  this->id = id;
  this->name = name;
  int hash = 0;
  int base = 0;
  char *p = id;
  while (*p) {
    hash += base + *p++;
    base += 75;
  }
  this->hash = hash;
  return this;
}

Symbol *symbol_new (char *name) {
  return symbol_new_id(name, name);
}

char *symbol_name (Symbol *this) {
  return this->name;
}

char *symbol_id (Symbol *this) {
  return this->id;
}

int symbol_hash (Symbol *this) {
  return this->hash;
}

Symbol *symbol_clone (Symbol *this) {
  Symbol *r = MALLOC(Symbol);
  r->id = this->id;
  r->name = this->name;
  r->hash = this->hash;
  return r;
}

int symbol_eq (Symbol *this, Symbol *other) {
  return this->hash == other->hash && str_eq(this->id, other->id);
}

char *symbol_to_str (Symbol *this) {
  return this->name;
}
