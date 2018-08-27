// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Either.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: @Either
  -msg: char *
  -value: void *
*/

/*.-.*/
struct either_Either {
  char *msg;
  void *value;
};

Either *_either_new(char *msg, void *value) {
  Either *this = MALLOC(Either);
  XNULL(msg)
  this->msg = msg;
  XNULL(value)
  this->value = value;
  return this;
}
/*.-.*/

Either *either_fail(char *msg) {
  if (!*msg) {
    exc_illegal_argument(msg, "No blank", "blank");
  }
  return _either_new(msg, msg);
}

/// Valor without errors.
Either *either_ok(void *value) {
  return _either_new("", value);
}

/// Returns true if 'this' is ok.
bool either_is_ok(Either *this) {
  return !*this->msg;
}

/// Returns true if 'this' is ok.
bool either_is_fail(Either *this) {
  return *this->msg;
}

/// Returns msg or throw an illegal_state exception if result was ok.
char *either_msg(Either *this) {
  if (either_is_ok(this)) {
    exc_illegal_state("Either is ok");
  }
  return this->msg;
}

/// Returns value or throw an illegal_state exception if result was fail.
void *either_value(Either *this) {
  if (!either_is_ok(this)) {
    exc_illegal_state("Either is fail");
  }
  return this->value;
}

