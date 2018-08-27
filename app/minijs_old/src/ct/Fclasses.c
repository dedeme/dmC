// Copyright 17-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include "dmc/exc.h"
#include "dmc/str.h"
#include "dmc/DEFS.h"
#include "io/Cpath.h"
#include "ast/Fclass.h"
#include "ct/Lfclass.h"
#include "ct/Ifclass.h"
#include "ct/Ofclass.h"

static Lfclass *classes = NULL;

void fclasses_init(void) {
  classes = lfclass_new();
}

bool fclasses_contains(Cpath *path) {
  char *id = cpath_id(path);
  /**/bool eq(Fclass *c) { return str_eq(fclass_id(c), id); }
  return ifclass_containsf(lfclass_to_it(classes), eq);
}

void fclasses_add(Fclass *c) {
  classes = lfclass_cons(classes, c);
}

Ofclass *fclasses_get(Cpath *path) {
  char *id = cpath_id(path);
  EACHL(classes, Fclass, c) {
    if (str_eq(id, fclass_id(c))) {
      return ofclass_new(c);
    }
  }_EACH
  return ofclass_null();
}
