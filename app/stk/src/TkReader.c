// Copyright 16-Apr-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "TkReader.h"

/* .
TkReader
  -reader: void *: _i
  -fsource: tkReader_Source: _i
  -fnext: tkReader_Next_new: _i
*/

/*--*/

struct tkReader_TkReader {
  void *reader;
  tkReader_Source fsource;
  tkReader_Next_new fnext;
};

TkReader *tkReader_new(void *reader, tkReader_Source fsource, tkReader_Next_new fnext) {
  TkReader *this = malloc(sizeof(TkReader));
  this->reader = reader;
  this->fsource = fsource;
  this->fnext = fnext;
  return this;
}

void tkReader_free(TkReader *this) {
  if (this) {
    free(this);
  }
};

/*--*/

char *tkReader_source (TkReader *this) {
  return this->fsource(this->reader);
}

/// Return the following token. When are no more tokens, returns a token
/// with value = "".
Token *tkReader_next_new (TkReader *this) {
  return this->fnext(this->reader);
}

