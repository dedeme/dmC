// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Implement.h"

/*.
struct: Implement
  pos: Pos *: pos
  path: char *: _string
  generics: Arr *: _array tp_str_type
*/

/*.-.*/
struct implement_Implement {
  Pos *pos;
  char *path;
  Arr *generics;
};

Implement *implement_new(Pos *pos, char *path, Arr *generics) {
  Implement *this = MALLOC(Implement);
  this->pos = pos;
  this->path = path;
  this->generics = generics;
  return this;
}

inline
Pos *implement_pos(Implement *this) {
  return this->pos;
}

inline
char *implement_path(Implement *this) {
  return this->path;
}

inline
Arr *implement_generics(Implement *this) {
  return this->generics;
}

Json *implement_serialize(Implement *this) {
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, pos_serialize(this->pos));
  jarr_astring(serial, this->path);
  jarr_aarray(serial, this->generics, (Json*(*)(void*)) tp_str_type_serialize);
  return json_warray(serial);
}

Implement *implement_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Implement *this = MALLOC(Implement);
  size_t i = 0;
  this->pos = pos_restore(arr_get(serial, i++));
  this->path = jarr_gstring(serial, i++);
  this->generics = jarr_garray(serial, i++, (void*(*)(Json*)) tp_str_type_restore);
  return this;
}
/*.-.*/
