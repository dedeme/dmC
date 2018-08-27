// Copyright 03-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "lexer/Pos.h"

/*.
struct: Pos
  nline: size_t: _uint
  nchar: size_t: _uint
*/

/*.-.*/
struct pos_Pos {
  size_t nline;
  size_t nchar;
};

Pos *pos_new(size_t nline, size_t nchar) {
  Pos *this = MALLOC(Pos);
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

size_t pos_nline(Pos *this) {
  XNULL(this)
  return this->nline;
}

size_t pos_nchar(Pos *this) {
  XNULL(this)
  return this->nchar;
}

Ajson *pos_to_json(Pos *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_auint(serial, this->nline);
  jarr_auint(serial, this->nchar);
  return serial;
}

Pos *pos_from_json(Ajson *serial) {
  XNULL(serial)
  Pos *this = MALLOC(Pos);
  size_t i = 0;
  this->nline = jarr_guint(serial, i++);
  this->nchar = jarr_guint(serial, i++);
  return this;
}
/*.-.*/
