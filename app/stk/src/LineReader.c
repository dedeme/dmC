// Copyright 21-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "LineReader.h"

int lineReader_line(LineReader *this) {
  return this->fnline(this);
}

char *lineReader_next_new(LineReader *this) {
  return this->fnext_new(this);
}

void lineReader_free(LineReader *this) {
  this->ffree(this);
}
