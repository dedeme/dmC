// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Fails.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: @Fails
  errors: Afail *
  warnings: Afail *
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct fails_Fails {
  Afail *errors;
  Afail *warnings;
};

Fails *_fails_new(Afail *errors, Afail *warnings) {
  Fails *this = MALLOC(Fails);
  XNULL(errors)
  this->errors = errors;
  XNULL(warnings)
  this->warnings = warnings;
  return this;
}

Afail *fails_errors(Fails *this) {
  XNULL(this)
  return this->errors;
}

Afail *fails_warnings(Fails *this) {
  XNULL(this)
  return this->warnings;
}
/*.-.*/

Fails *fails_new() {
  return _fails_new(afail_new(), afail_new());
}

Fails *fails_new_error(Fail *e) {
  Afail *a = afail_new();
  afail_add(a, e);
  return _fails_new(a, afail_new());
}

Fails *fails_new_warning(Fail *e) {
  Afail *a = afail_new();
  afail_add(a, e);
  return _fails_new(afail_new(), a);
}

bool fails_is_error(Fails *this) {
  return afail_size(this->errors);
}

void fails_add_error(Fails *this, Fail *e) {
  afail_add_if(this->errors, e);
}

void fails_add_errors(Fails *this, Afail *es) {
  afail_add_arr_if(this->errors, es);
}

void fails_add_warning(Fails *this, Fail *w) {
  afail_add_if(this->warnings, w);
}

void fails_add_warnings(Fails *this, Afail *ws) {
  afail_add_arr_if(this->warnings, ws);
}
