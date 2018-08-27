// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "io/Cpath.h"
#include "ast/Class.h"
#include "ct/Lclass.h"
#include "ct/Iclass.h"
#include "ct/Oclass.h"

static Lclass *classes = NULL;

void classes_init(void) {
  classes = lclass_new();
}

bool classes_contains(Cpath *path) {
  char *id = cpath_id(path);
  /**/bool eq(Class *c) { return str_eq(class_id(c), id); }
  return iclass_containsf(lclass_to_it(classes), eq);
}

void classes_add(Class *c) {
  classes = lclass_cons(classes, c);
}

Oclass *classes_get(Cpath *path) {
  char *id = cpath_id(path);
  EACHL(classes, Class, c) {
    if (str_eq(id, class_id(c))) {
      return oclass_new(c);
    }
  }_EACH
  return oclass_null();
}
