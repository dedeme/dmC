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


JsEntryResult *entry_reading_run (
  UserData *userData, Errors *errors, Dep *function
) {

  Ojson *ojs = io_read_class(dep_class(function));
  JsClassResult *cl_rs = ojson_is_null(ojs)
    ? class_transpilation_run(userData, errors, dep_class(function))
    : jsClassResult_new(
        errors_new(lerror_new(), lerror_new()),
        jsClass_from_json(ojson_value(ojs))
      );

  Errors *cerrors = jsClassResult_errors(cl_rs);
  JsClass *class = jsClassResult_class(cl_rs);
  if (lerror_empty(errors_errors(cerrors))) {
    MjsEntry *entries = jsClass_entries(class);
    OjsEntry *oentry = mjsEntry_get(entries, dep_entry(function));
    if (ojsEntry_is_null(oentry)) {
      char *file = cpath_relative(dep_class(function));
      char *msg = str_printf("Entry '%s'.main not found", dep_entry(function));
      Error *e = error_new(msg, file, 0, 0);
      errors = errors_add_error(errors, e);
      return jsEntryResult_fail(errors);
    }
    JsEntry *entry = ojsEntry_value(oentry);
    errors = errors_add_warnings(errors, errors_warnings(cerrors));
    return jsEntryResult_new(errors, entry);
  }

  errors = errors_add_errors(errors, errors_warnings(cerrors));
  return jsEntryResult_fail(errors);
}

