// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/JsEntryResult.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: JsEntryResult
  fails: Fails *
  -entry: JsEntry *
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct jsEntryResult_JsEntryResult {
  Fails *fails;
  JsEntry *entry;
};

JsEntryResult *jsEntryResult_new(Fails *fails, JsEntry *entry) {
  JsEntryResult *this = MALLOC(JsEntryResult);
  XNULL(fails)
  this->fails = fails;
  XNULL(entry)
  this->entry = entry;
  return this;
}

Fails *jsEntryResult_fails(JsEntryResult *this) {
  XNULL(this)
  return this->fails;
}
/*.-.*/

JsEntryResult *jsEntryResult_error(Fails *fails) {
  XNULL(fails)
  JsEntryResult *this = MALLOC(JsEntryResult);
  this->fails = fails;
  this->entry = NULL;
  return this;
}

JsEntry *jsEntryResult_entry(JsEntryResult *this) {
  XNULL(this);
  if (!afail_size(fails_errors(this->fails))) {
    return this->entry;
  }
  THROW(exc_illegal_state_t) "The JsEntryResult contains errors" _THROW
  return NULL; // Unreachable statement.
}
