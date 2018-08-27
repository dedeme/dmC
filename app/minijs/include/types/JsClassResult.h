// Copyright 25-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Record to retrieve an result of reading an JsClass.
/// <p>
/// Fields:
///   errors: Errors in reading
///   class: jsClass read.
#ifndef TYPES_JSECLASSRESULT_H
  # define TYPES_JSECLASSRESULT_H

#include "Errors.h"
#include "JsClass.h"

/*.-.*/

///
typedef struct jsClassResult_JsClassResult JsClassResult;

///
JsClassResult *jsClassResult_new(Errors *errors, JsClass *class);

///
Errors *jsClassResult_errors(JsClassResult *this);

/*.-.*/

/// Constructor of errors is known to have fails.
JsClassResult *jsClassResult_fail(Errors *errors);

/// Returns a class if there is no error, otherwise throws an illegal_state
/// Exception.
JsClass *jsClassResult_class(JsClassResult *this);

#endif

