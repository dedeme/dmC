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


Fails *transpilation_run (UserData *userData) {
  Lchar *roots = userData_roots(userData);
  Dep *mainFunction = dep_new(userData_source(userData), "main");
  JsEntryResult *r = entry_reading_run(roots, mainFunction); // IN CURSE

  Fails *fails = jsEntryResult_fails(r);
  if (afail_size(fails_errors(fails))) {
    return fails;
  }
  JsEntry *entry = jsEntryResult_entry(r);

  Type *mainType = type_new(true, "", atype_new());
  if (!type_eq(mainType, jsEntry_type(entry))) {
    char *file = cpath_relative(userData_source(userData));
    char *msg = str_printf("Entry '%s'.main is not of type '()'", file);
    Fail *e = fail_new(msg, file, 0, 0);
    Afail *afs = afail_new();
    afail_add(afs, e);
    return fails_new(afs, afail_new());
  }

  js_writing_run(userData, entry);

  return jsEntryResult_fails(r);
}
