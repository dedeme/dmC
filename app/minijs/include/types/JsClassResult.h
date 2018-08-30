// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Record to retrieve an result of reading a JsClass.
/// <p>
/// Fields:
///   errors: Errors in reading
///   class: jsClass read.
#ifndef TYPES_JSCLASSRESULT_H
  # define TYPES_JSCLASSRESULT_H

#include "Fails.h"
#include "JsClass.h"

/*.-.*/

///
typedef struct jsClassResult_JsClassResult JsClassResult;

///
JsClassResult *jsClassResult_new(Fails *fails, JsClass *class);

///
Fails *jsClassResult_fails(JsClassResult *this);

/*.-.*/

/// Constructor when fails is known to have errors.
JsClassResult *jsClassResult_error(Fails *fails);

/// Returns a class if there is no error, otherwise throws an illegal_state
/// Exception.
JsClass *jsClassResult_class(JsClassResult *this);

#endif

