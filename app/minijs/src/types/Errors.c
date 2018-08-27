// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Errors.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: Errors
  errors: Lerror *
  warnings: Lerror *
*/

/*.-.*/
struct errors_Errors {
  Lerror *errors;
  Lerror *warnings;
};

Errors *errors_new(Lerror *errors, Lerror *warnings) {
  Errors *this = MALLOC(Errors);
  XNULL(errors)
  this->errors = errors;
  XNULL(warnings)
  this->warnings = warnings;
  return this;
}

Lerror *errors_errors(Errors *this) {
  XNULL(this)
  return this->errors;
}

Lerror *errors_warnings(Errors *this) {
  XNULL(this)
  return this->warnings;
}
/*.-.*/

Errors *errors_add_error(Errors *this, Error *e) {
  return errors_new(lerror_add_if(this->errors, e), this->warnings);
}

Errors *errors_add_errors(Errors *this, Lerror *es) {
  return errors_new(lerror_add_list(this->errors, es), this->warnings);
}

Errors *errors_add_warning(Errors *this, Error *w) {
  return errors_new(this->errors, lerror_add_if(this->warnings, w));
}

Errors *errors_add_warnings(Errors *this, Lerror *ws) {
  return errors_new(this->errors, lerror_add_list(this->warnings, ws));
}
