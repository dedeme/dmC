// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Scanner.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: @Scanner
  file: Cpath *
  tx: char *
  ix: size_t
  nline: int
  nchar: int
*/

/*.-.*/
struct scanner_Scanner {
  Cpath *file;
  char *tx;
  size_t ix;
  int nline;
  int nchar;
};

Scanner *_scanner_new(
  Cpath *file,
  char *tx,
  size_t ix,
  int nline,
  int nchar
) {
  Scanner *this = MALLOC(Scanner);
  XNULL(file)
  this->file = file;
  XNULL(tx)
  this->tx = tx;
  this->ix = ix;
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

Cpath *scanner_file(Scanner *this) {
  XNULL(this)
  return this->file;
}

char *scanner_tx(Scanner *this) {
  XNULL(this)
  return this->tx;
}

size_t scanner_ix(Scanner *this) {
  XNULL(this)
  return this->ix;
}

int scanner_nline(Scanner *this) {
  XNULL(this)
  return this->nline;
}

int scanner_nchar(Scanner *this) {
  XNULL(this)
  return this->nchar;
}
/*.-.*/

Scanner *scanner_new(Cpath *file, char *text) {
  return _scanner_new(file, text, 0, 1, 0);
}

Scanner *scanner_new_text(char *text) {
  return _scanner_new(cpath_new("", "Text"), text, 0, 1, 0);
}

bool scanner_eq(Scanner *this, Scanner *other) {
  XNULL(this)
  XNULL(other)
  return this->tx == other->tx && this->ix == other->ix;
}

Scanner *scanner_next(Scanner *this) {
  char ch = this->tx[this->ix];
  if (ch) {
    int nline = this->nline;
    int nchar = this->nchar + 1;
    if (ch == '\n') {
      ++nline;
      nchar = 0;
    }
    return _scanner_new(this->file, this->tx, this->ix + 1, nline, nchar);
  }
  return this;
}

char scanner_char(Scanner *this) {
  return this->tx[this->ix];
}

bool scanner_is_end(Scanner *this) {
  return !this->tx[this->ix];
}

Fail *scanner_mk_fail(Scanner *this, char *msg) {
  return fail_new(msg, cpath_relative(this->file), this->nline, this->nchar);
}
