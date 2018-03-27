// Copyright 23-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "lexer/Filepos.h"

/*.
struct: Filepos
  path: char *: _string
  nline: size_t: _uint
  nchar: size_t: _uint
*/

/*.-.*/
struct filepos_Filepos {
  char *path;
  size_t nline;
  size_t nchar;
};

Filepos *filepos_new(char *path, size_t nline, size_t nchar) {
  Filepos *this = MALLOC(Filepos);
  this->path = path;
  this->nline = nline;
  this->nchar = nchar;
  return this;
}

inline
char *filepos_path(Filepos *this) {
  return this->path;
}

inline
size_t filepos_nline(Filepos *this) {
  return this->nline;
}

inline
size_t filepos_nchar(Filepos *this) {
  return this->nchar;
}

Json *filepos_serialize(Filepos *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_astring(serial, this->path);
  jarr_auint(serial, this->nline);
  jarr_auint(serial, this->nchar);
  return json_warray(serial);
}

Filepos *filepos_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Filepos *this = MALLOC(Filepos);
  size_t i = 0;
  this->path = jarr_gstring(serial, i++);
  this->nline = jarr_guint(serial, i++);
  this->nchar = jarr_guint(serial, i++);
  return this;
}
/*.-.*/
