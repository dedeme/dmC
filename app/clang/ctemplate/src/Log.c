// Copyright 24-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Log.h"

struct log_Log {
  char *file;
  int nline;
};

Log *log_new(Gc *gc, char *file) {
  Log *this = GC_MALLOC(Log);
  this->file = file;
  this->nline = 0;
  return this;
}

void log_inc(Log *this) {
  ++this->nline;
}

void log_msg(Log *this, char *code, char *msg) {
  GC_NEW
  printf(
    "%s:%d: %s\n  in '%s'\n\n",
    this->file, this->nline, msg, str_trim(gc, code)
  );
  GC_FREE
}
