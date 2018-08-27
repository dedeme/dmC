// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "scan/transpilation.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "types/Type.h"
#include "types/Dep.h"
#include "types/JsEntryResult.h"
#include "scan/entry_reading.h"
#include "js_writing.h"


Errors *transpilation_run (UserData *userData) {
  Dep *mainFunction = dep_new(userData_source(userData), "main");
  Errors *errors = errors_new(lerror_new(), lerror_new());
  JsEntryResult *r = entry_reading_run(userData, errors, mainFunction); // IN CURSE

  errors = jsEntryResult_errors(r);
  JsEntry *entry = jsEntryResult_entry(r);

  if (!lerror_empty(errors_errors(errors))) {
    return errors;
  }

  Type *mainType = type_new(true, "", atype_new());
  if (!type_eq(mainType, jsEntry_type(entry))) {
    char *file = cpath_relative(userData_source(userData));
    char *msg = str_printf("Entry '%s'.main is not of type '()'", file);
    Error *e = error_new(msg, file, 0, 0);
    Lerror *les = lerror_cons(lerror_new(), e);
    return errors_new(les, lerror_new());
  }

  js_writing_run(userData, entry);  // TO DO

  return jsEntryResult_errors(r);
}
