// Copyright 01-Ap-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/TpStrType.h"

/*.
struct: Tp_str_type
  s: char *: _string
  t: Type *: type
*/

/*.-.*/
struct tp_str_type_Tp_str_type {
  char *s;
  Type *t;
};

Tp_str_type *tp_str_type_new(char *s, Type *t) {
  Tp_str_type *this = MALLOC(Tp_str_type);
  this->s = s;
  this->t = t;
  return this;
}

inline
char *tp_str_type_s(Tp_str_type *this) {
  return this->s;
}

inline
Type *tp_str_type_t(Tp_str_type *this) {
  return this->t;
}

Json *tp_str_type_serialize(Tp_str_type *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_astring(serial, this->s);
  arr_add(serial, type_serialize(this->t));
  return json_warray(serial);
}

Tp_str_type *tp_str_type_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Tp_str_type *this = MALLOC(Tp_str_type);
  size_t i = 0;
  this->s = jarr_gstring(serial, i++);
  this->t = type_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/
