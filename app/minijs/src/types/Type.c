// Copyright 18-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types/Type.h"
#include "lexer/token.h"

/*.
struct: Type
  type: enum Type_t: _uint
  id:char *: _string
  params: Arr *: _array type
*/

/*.-.*/
struct type_Type {
  enum Type_t type;
  char *id;
  Arr *params;
};

Type *type_new(enum Type_t type, char *id, Arr *params) {
  Type *this = MALLOC(Type);
  this->type = type;
  this->id = id;
  this->params = params;
  return this;
}

inline
enum Type_t type_type(Type *this) {
  return this->type;
}

inline
char *type_id(Type *this) {
  return this->id;
}

inline
Arr *type_params(Type *this) {
  return this->params;
}

Json *type_serialize(Type *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->type);
  jarr_astring(serial, this->id);
  jarr_aarray(serial, this->params, (Json*(*)(void*)) type_serialize);
  return json_warray(serial);
}

Type *type_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Type *this = MALLOC(Type);
  size_t i = 0;
  this->type = jarr_guint(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->params = jarr_garray(serial, i++, (void*(*)(Json*)) type_restore);
  return this;
}
/*.-.*/

bool type_eq(Type *this, Type *other) {
  if (this->type != other->type) {
    return false;
  }
  if (strcmp(this->id, other->id)) {
    return false;
  }

  if (arr_size(this->params) != arr_size(other->params)) {
    return false;
  }

  RANGE0(i, arr_size(this->params)) {
    if (!type_eq(arr_get(this->params, i), arr_get(other->params, i))) {
      return false;
    }
  }_RANGE

  return true;
}
