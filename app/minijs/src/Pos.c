// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Pos.h"

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

inline
size_t pos_nline(Pos *this) {
  return this->nline;
}

inline
size_t pos_nchar(Pos *this) {
  return this->nchar;
}

Json *pos_serialize(Pos *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->nline);
  jarr_auint(serial, this->nchar);
  return json_warray(serial);
}

Pos *pos_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Pos *this = MALLOC(Pos);
  size_t i = 0;
  this->nline = jarr_guint(serial, i++);
  this->nchar = jarr_guint(serial, i++);
  return this;
}
/*.-.*/
