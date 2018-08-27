// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Deps.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "dmc/Arr.h"

struct deps_Deps {
  Arr/*Dep*/ *deps;
};

Deps *deps_new(void) {
  Deps *this = MALLOC(Deps);
  this->deps = arr_new();
  return this;
}

/// Add 'dep' if it does not exist
void deps_add(Deps *this, Dep *dep) {
  /**/bool eq(void *e) {
  /**/  return dep_eq(e, dep);
  /**/}
  if (it_contains(arr_to_it(this->deps), eq)) {
    return;
  }
  arr_add(this->deps, dep);
}

///
size_t deps_size(Deps *this) {
  return arr_size(this->deps);
}

///
It/*Dep*/ *deps_to_it(Deps *this) {
  return arr_to_it(this->deps);
}

///
Json *deps_to_json(Deps *this) {
  return arr_to_json(this->deps, (TO_JSON)dep_to_json);
}

///
Deps *deps_from_json(Json *js) {
  Deps *this = MALLOC(Deps);
  this->deps = arr_from_json(js, (FROM_JSON)dep_from_json);
  return this;
}
