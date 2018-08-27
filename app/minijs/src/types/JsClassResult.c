// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/JsClassResult.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: JsClassResult
  errors: Errors *
  -class: JsClass *
*/

/*.-.*/
struct jsClassResult_JsClassResult {
  Errors *errors;
  JsClass *class;
};

JsClassResult *jsClassResult_new(Errors *errors, JsClass *class) {
  JsClassResult *this = MALLOC(JsClassResult);
  XNULL(errors)
  this->errors = errors;
  XNULL(class)
  this->class = class;
  return this;
}

Errors *jsClassResult_errors(JsClassResult *this) {
  XNULL(this)
  return this->errors;
}
/*.-.*/

JsClassResult *jsClassResult_fail(Errors *errors) {
  JsClassResult *this = MALLOC(JsClassResult);
  XNULL(errors)
  this->errors = errors;
  this->class = NULL;
  return this;
}

JsClass *jsClassResult_class(JsClassResult *this) {
  XNULL(this);
  if (lerror_empty(errors_errors(this->errors))) {
    return this->class;
  }
  THROW(exc_illegal_state_t) "The JsClassResult contains errors" _THROW
  return NULL; // Unreachable statement.
}
