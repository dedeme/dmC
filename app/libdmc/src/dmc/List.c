// Copyright 29-May-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/List.h"
#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"

struct list_List {
  List *next;
  void *value;
};

List *list_new(void) {
  List *this = MALLOC(List);
  this->next = NULL;
  this->value = NULL;
  return this;
}

List *list_tail(List *this) {
  if (this->next) {
    return this->next;
  }
  THROW(exc_illegal_argument_t)
    exc_illegal_argument ("this", "not empty", "empty")
  _THROW
  return NULL;
}

void *list_head (List *this) {
  if (this->next) {
    return this->value;
  }
  THROW(exc_illegal_argument_t)
    exc_illegal_argument ("this", "not empty", "empty") _THROW
  return NULL;
}

List *list_cons(List *old, void *o) {
  List *this = MALLOC(List);
  this->next = old;
  this->value = o;
  return this;
}

