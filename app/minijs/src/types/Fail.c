// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "types/Fail.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"

/*.
-struct: Fail
  msg: char *
  file: char *
  nline: int
  nchar: int
*/

/*.-.*/
#include "dmc/ct/Ajson.h"

struct fail_Fail {
  char *msg;
  char *file;
  int nline;
  int nchar;
};

Fail *fail_new(
  char *msg,
  char *file,
  int nline,
  int nchar
) {
  Fail *this = MALLOC(Fail);
  XNULL(msg)
  this->msg = msg;
  XNULL(file)
  this->file = file;
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

char *fail_msg(Fail *this) {
  XNULL(this)
  return this->msg;
}

char *fail_file(Fail *this) {
  XNULL(this)
  return this->file;
}

int fail_nline(Fail *this) {
  XNULL(this)
  return this->nline;
}

int fail_nchar(Fail *this) {
  XNULL(this)
  return this->nchar;
}
/*.-.*/

char *fail_fmsg(Fail *this) {
  return str_printf("%s[%d, %d]: %s",
    this->file, this->nline, this->nchar, this->msg
  );
}
