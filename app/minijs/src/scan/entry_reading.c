// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "scan/entry_reading.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "dmc/ct/Ojson.h"
#include "types/Dep.h"
#include "types/JsEntryResult.h"
#include "types/JsClassResult.h"
#include "types/OjsEntry.h"
#include "io.h"
#include "scan/class_transpilation.h"


JsEntryResult *entry_reading_run (Lchar *roots, Dep *function) {

  Ojson *ojs = io_read_class(dep_class(function));
  JsClassResult *cl_rs = ojson_is_null(ojs)
    ? class_transpilation_run(roots, dep_class(function))
    : jsClassResult_new(fails_new(), jsClass_from_json(ojson_value(ojs)));

  Fails *fails = jsClassResult_fails(cl_rs);

  if (!afail_size(fails_errors(fails))) {
    JsClass *class = jsClassResult_class(cl_rs);
    MjsEntry *entries = jsClass_entries(class);
    OjsEntry *oentry = mjsEntry_get(entries, dep_entry(function));
    if (ojsEntry_is_null(oentry)) {
      char *file = cpath_relative(dep_class(function));
      char *msg = str_printf("Entry '%s'.main not found", dep_entry(function));
      Fail *e = fail_new(msg, file, 0, 0);
      fails_add_error(fails, e);
      return jsEntryResult_error(fails);
    }
    JsEntry *entry = ojsEntry_value(oentry);
    return jsEntryResult_new(fails, entry);
  }

  return jsEntryResult_error(fails);
}

