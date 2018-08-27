// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/JsEntryResult.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: JsEntryResult
  errors: Errors *
  -entry: JsEntry *
*/

/*.-.*/
struct jsEntryResult_JsEntryResult {
  Errors *errors;
  JsEntry *entry;
};

JsEntryResult *jsEntryResult_new(Errors *errors, JsEntry *entry) {
  JsEntryResult *this = MALLOC(JsEntryResult);
  XNULL(errors)
  this->errors = errors;
  XNULL(entry)
  this->entry = entry;
  return this;
}

Errors *jsEntryResult_errors(JsEntryResult *this) {
  XNULL(this)
  return this->errors;
}
/*.-.*/

JsEntryResult *jsEntryResult_fail(Errors *errors) {
  JsEntryResult *this = MALLOC(JsEntryResult);
  XNULL(errors)
  this->errors = errors;
  this->entry = NULL;
  return this;
}

JsEntry *jsEntryResult_entry(JsEntryResult *this) {
  XNULL(this);
  if (lerror_empty(errors_errors(this->errors))) {
    return this->entry;
  }
  THROW(exc_illegal_state_t) "The JsEntryResult contains errors" _THROW
  return NULL; // Unreachable statement.
}
