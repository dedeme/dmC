// Copyright 24-Feb-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "reader/types.h"

struct types_Types {
  // <char *> Array of C array with two values: [symbol, module]
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

void types_add (Types *this, char *symbol, char *module) {
  char **val = GC_MALLOC(2 * sizeof(char *));
  val[0] = symbol;
  val[1] = module;

  arr_push(this->block, val);
}

int types_in_block (Types *this, char *sym) {
  EACH(this->block, char *, e) {
    if (!strcmp(e[0], sym)) return TRUE;
  }_EACH
  return FALSE;
}

char *types_find (Types *this, char *sym) {
  for (;;) {
    EACH(this->block, char *, e) {
      if (!strcmp(e[0], sym)) return e[1];
    }_EACH
    if (!this->next) break;
    this = this->next;
  }
  return "";
}

int types_contains(Types *this, char *module) {
  for (;;) {
    EACH(this->block, char *, e) {
      if (!strcmp(e[1], module)) return TRUE;
    }_EACH
    if (!this->next) break;
    this = this->next;
  }
  return FALSE;
}
