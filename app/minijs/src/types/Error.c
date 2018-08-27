// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Error.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: Error
  msg: char *
  file: char *
  nline: int
  nchar: int
*/

/*.-.*/
struct error_Error {
  char *msg;
  char *file;
  int nline;
  int nchar;
};

Error *error_new(
  char *msg,
  char *file,
  int nline,
  int nchar
) {
  Error *this = MALLOC(Error);
  XNULL(msg)
  this->msg = msg;
  XNULL(file)
  this->file = file;
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

char *error_msg(Error *this) {
  XNULL(this)
  return this->msg;
}

char *error_file(Error *this) {
  XNULL(this)
  return this->file;
}

int error_nline(Error *this) {
  XNULL(this)
  return this->nline;
}

int error_nchar(Error *this) {
  XNULL(this)
  return this->nchar;
}
/*.-.*/

char *error_fmsg(Error *this) {
  return str_printf("%s[%d, %d]: %s",
    this->file, this->nline, this->nchar, this->msg
  );
}
