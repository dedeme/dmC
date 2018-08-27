// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Wrapper for result which can fail
/// <p>
/// Fields: Opaque fields
#ifndef TYPES_EITHER_H
  # define TYPES_EITHER_H

#include "stdbool.h"

/*.-.*/

///
typedef struct either_Either Either;

/*.-.*/

/// Fail with error 'msg'
Either *either_fail(char *msg);

/// Valor without errors.
Either *either_ok(void *value);

/// Returns true if 'this' is ok.
bool either_is_ok(Either *this);

/// Returns true if 'this' is fail.
bool either_is_fail(Either *this);

/// Returns msg or throw an illegal_state exception if result was ok.
char *either_msg(Either *this);

/// Returns value or throw an illegal_state exception if result was fail.
void *either_value(Either *this);

#endif

