// Copyright 27-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/ScanResult.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: ScanResult
  fails: Fails *
  -result: void *
  -scanner: Scanner *
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct scanResult_ScanResult {
  Fails *fails;
  void *result;
  Scanner *scanner;
};

ScanResult *scanResult_new(Fails *fails, void *result, Scanner *scanner) {
  ScanResult *this = MALLOC(ScanResult);
  XNULL(fails)
  this->fails = fails;
  XNULL(result)
  this->result = result;
  XNULL(scanner)
  this->scanner = scanner;
  return this;
}

Fails *scanResult_fails(ScanResult *this) {
  XNULL(this)
  return this->fails;
}
/*.-.*/

ScanResult *scanResult_new_ok(void *result, Scanner *scanner) {
  return scanResult_new(fails_new(), result, scanner);
}

ScanResult *scanResult_error(Fails *fails) {
  ScanResult *this = MALLOC(ScanResult);
  XNULL(fails)
  this->fails = fails;
  this->result = NULL;
  this->scanner = NULL;
  return this;
}

void *scanResult_result(ScanResult *this) {
  XNULL(this);
  if (!afail_size(fails_errors(this->fails))) {
    return this->result;
  }
  THROW(exc_illegal_state_t) "ScanResult contains errors" _THROW
  return NULL; // Unreachable statement.
}

Scanner *scanResult_scanner(ScanResult *this) {
  XNULL(this);
  if (!afail_size(fails_errors(this->fails))) {
    return this->scanner;
  }
  THROW(exc_illegal_state_t) "ScanResult contains errors" _THROW
  return NULL; // Unreachable statement.
}

bool scanResult_is_error(ScanResult *this) {
  return fails_is_error(this->fails);
}

Afail *scanResult_errors(ScanResult *this) {
  return fails_errors(this->fails);
}

Afail *scanResult_warnings(ScanResult *this) {
  return fails_warnings(this->fails);
}

