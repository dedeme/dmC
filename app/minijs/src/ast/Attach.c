// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Attach.h"

/*.
struct:@Attach
  type: enum Attach_t: _uint
  id: char *: _string
  values: Arr *: _array value
*/

/*.-.*/
struct attach_Attach {
  enum Attach_t type;
  char *id;
  Arr *values;
};

Attach *_attach_new(enum Attach_t type, char *id, Arr *values) {
  Attach *this = MALLOC(Attach);
  this->type = type;
  this->id = id;
  this->values = values;
  return this;
}

inline
enum Attach_t attach_type(Attach *this) {
  return this->type;
}

inline
char *attach_id(Attach *this) {
  return this->id;
}

inline
Arr *attach_values(Attach *this) {
  return this->values;
}

Json *attach_serialize(Attach *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->type);
  jarr_astring(serial, this->id);
  jarr_aarray(serial, this->values, (Json*(*)(void*)) value_serialize);
  return json_warray(serial);
}

Attach *attach_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Attach *this = MALLOC(Attach);
  size_t i = 0;
  this->type = jarr_guint(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->values = jarr_garray(serial, i++, (void*(*)(Json*)) value_restore);
  return this;
}
/*.-.*/

inline
Attach *attach_new_dot(char *id) {
  return _attach_new(ADOT, id, arr_new());
}

Attach *attach_new_sub(Value *v) {
  Arr/*Value*/ *vs = arr_new();
  arr_add(vs, v);
  return _attach_new(ASUB, "", vs);
}

inline
Attach *attach_new_fn(Arr/*Value*/ *vs) {
  return _attach_new(AFN, "", vs);
}
