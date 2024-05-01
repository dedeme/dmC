// Copyright 24-Feb-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/types.h"

struct types_Types {
  // <int> Array of C array with two values: [symbol, module]
  Arr *block;
  struct types_Types *next;
};

Types *types_new (void) {
  Types *this = MALLOC(Types);
  this->block = arr_new();
  this->next = NULL;
  return this;
}

Types *types_new_block (Types *this) {
  Types *r = types_new();
  r->next = this;
  return r;
}

void types_add (Types *this, int symbol, int module) {
  int *val = ATOMIC(2 * sizeof(int));
  val[0] = symbol;
  val[1] = module;

  arr_push(this->block, val);
}

int types_in_block (Types *this, int sym) {
  EACH(this->block, int, e) {
    if (e[0] == sym) return TRUE;
  }_EACH
  return FALSE;
}

int types_find (Types *this, int sym) {
  for (;;) {
    EACH(this->block, int, e) {
      if (e[0] == sym) return e[1];
    }_EACH
    if (!this->next) break;
    this = this->next;
  }
  return -1;
}

int types_contains(Types *this, int module) {
  for (;;) {
    EACH(this->block, int, e) {
      if (e[1] == module) return TRUE;
    }_EACH
    if (!this->next) break;
    this = this->next;
  }
  return FALSE;
}
