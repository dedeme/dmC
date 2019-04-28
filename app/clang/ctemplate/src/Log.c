// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Log.h"

struct log_Log {
  char *file;
  int nline;
};

Log *log_new(char *file) {
  Log *this = MALLOC(Log);
  this->file = file;
  this->nline = 0;
  return this;
}

void log_inc(Log *this) {
  ++this->nline;
}

void log_msg(Log *this, char *code, char *msg) {
  printf(
    "%s:%d: %s\n  in '%s'\n\n",
    this->file, this->nline, msg, str_trim(code)
  );
}
