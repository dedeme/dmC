// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Record to retrieve an result of reading a class skeleton.
/// <p>
/// Fields:
///   errors: Errors in reading
///   Scanner: With final state
///   result: Object read if there are not errors.
#ifndef TYPES_SCANRESULT_H
  # define TYPES_SCANRESULT_H

#include "Fails.h"
#include "Scanner.h"
#include "stdbool.h"

/*.-.*/

///
typedef struct scanResult_ScanResult ScanResult;

///
ScanResult *scanResult_new(Fails *fails, void *result, Scanner *scanner);

///
Fails *scanResult_fails(ScanResult *this);

/*.-.*/

/// Constructor wihout errors or warnings.
ScanResult *scanResult_new_ok(void *result, Scanner *scanner);

/// Constructor of a fails with errors.
ScanResult *scanResult_error(Fails *fails);

/// Returns a result if there is no error, otherwise throws an illegal_state
/// Exception.
void *scanResult_result(ScanResult *this);

/// Returns a scanner if there is no error, otherwise throws an illegal_state
/// Exception.
Scanner *scanResult_scanner(ScanResult *this);

/// Returns true if the reading resulted in errors
bool scanResult_is_error(ScanResult *this);

/// Returns errors of this.
Afail *scanResult_errors(ScanResult *this);

/// Returns warnings of this.
Afail *scanResult_warnings(ScanResult *this);

#endif

