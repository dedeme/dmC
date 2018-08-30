// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Record to retrieve an result of reading an JsEntry.
/// <p>
/// Fields:
///   errors: Errors in reading
///   entry: jsEntry read.
#ifndef TYPES_JSENTRYRESULT_H
  # define TYPES_JSENTRYRESULT_H

#include "Fails.h"
#include "JsEntry.h"

/*.-.*/

///
typedef struct jsEntryResult_JsEntryResult JsEntryResult;

///
JsEntryResult *jsEntryResult_new(Fails *fails, JsEntry *entry);

///
Fails *jsEntryResult_fails(JsEntryResult *this);

/*.-.*/

/// Constructor of errors is known to have errors.
JsEntryResult *jsEntryResult_error(Fails *fails);


/// Returns an entry if there is no error, otherwise throws an illegal_state
/// Exception.
JsEntry *jsEntryResult_entry(JsEntryResult *this);

#endif

