// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/JsClassResult.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: JsClassResult
  fails: Fails *
  -class: JsClass *
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct jsClassResult_JsClassResult {
  Fails *fails;
  JsClass *class;
};

JsClassResult *jsClassResult_new(Fails *fails, JsClass *class) {
  JsClassResult *this = MALLOC(JsClassResult);
  XNULL(fails)
  this->fails = fails;
  XNULL(class)
  this->class = class;
  return this;
}

Fails *jsClassResult_fails(JsClassResult *this) {
  XNULL(this)
  return this->fails;
}
/*.-.*/

JsClassResult *jsClassResult_error(Fails *fails) {
  XNULL(fails)
  JsClassResult *this = MALLOC(JsClassResult);
  this->fails = fails;
  this->class = NULL;
  return this;
}

JsClass *jsClassResult_class(JsClassResult *this) {
  XNULL(this);
  if (!afail_size(fails_errors(this->fails))) {
    return this->class;
  }
  THROW(exc_illegal_state_t) "The JsClassResult contains errors" _THROW
  return NULL; // Unreachable statement.
}
