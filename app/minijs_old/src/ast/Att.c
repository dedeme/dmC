// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Att.h"

/*.
struct: Att
  is_public: bool: _bool
  id:char *: _string
  type: Type *: type
  is_val: bool: _bool
  -_value: Value *: value
*/

/*.-.*/
struct att_Att {
  bool is_public;
  char *id;
  Type *type;
  bool is_val;
  Value *_value;
};

Att *att_new(
  bool is_public,
  char *id,
  Type *type,
  bool is_val,
  Value *_value
) {
  Att *this = MALLOC(Att);
  this->is_public = is_public;
  this->id = id;
  this->type = type;
  this->is_val = is_val;
  this->_value = _value;
  return this;
}

inline
bool att_is_public(Att *this) {
  return this->is_public;
}

inline
char *att_id(Att *this) {
  return this->id;
}

inline
Type *att_type(Att *this) {
  return this->type;
}

inline
bool att_is_val(Att *this) {
  return this->is_val;
}

Arr/*Json*/ *att_serialize(Att *this) {
  Arr/*Json*/ *serial = arr_new();
  if (!this) return serial;
  jarr_abool(serial, this->is_public);
  jarr_astring(serial, this->id);
  arr_add(serial, json_warray(type_serialize(this->type)));
  jarr_abool(serial, this->is_val);
  arr_add(serial, json_warray(value_serialize(this->_value)));
  return serial;
}

Att *att_restore(Arr/*Json*/ *serial) {
  if (!arr_size(serial)) return NULL;
  Att *this = MALLOC(Att);
  size_t i = 0;
  this->is_public = jarr_gbool(serial, i++);
  this->id = jarr_gstring(serial, i++);
  this->type = type_restore(json_rarray(arr_get(serial, i++)));
  this->is_val = jarr_gbool(serial, i++);
  this->_value = value_restore(json_rarray(arr_get(serial, i++)));
  return this;
}
/*.-.*/

Value *att_value(Att *this) {
  if (!this->_value)
    THROW exc_null_pointer("this->value") _THROW
  return this->_value;
}
