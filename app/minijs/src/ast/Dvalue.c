// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Dvalue.h"

/*.
struct: @Dvalue
  pos: Filepos *: filepos
  id: char *: _string
  +tpos: Filepos *: filepos
  +type: Type *: type
  +value: Value *: value
*/

/*.-.*/
struct dvalue_Dvalue {
  Filepos *pos;
  char *id;
  Filepos *tpos;
  Type *type;
  Value *value;
};

Dvalue *_dvalue_new(
  Filepos *pos,
  char *id,
  Filepos *tpos,
  Type *type,
  Value *value
) {
  Dvalue *this = MALLOC(Dvalue);
  this->pos = pos;
  this->id = id;
  this->tpos = tpos;
  this->type = type;
  this->value = value;
  return this;
}

inline
Filepos *dvalue_pos(Dvalue *this) {
  return this->pos;
}

inline
char *dvalue_id(Dvalue *this) {
  return this->id;
}

inline
Filepos *dvalue_tpos(Dvalue *this) {
  return this->tpos;
}

inline
void dvalue_set_tpos(Dvalue *this, Filepos *value) {
  this->tpos = value;
}

inline
Type *dvalue_type(Dvalue *this) {
  return this->type;
}

inline
void dvalue_set_type(Dvalue *this, Type *value) {
  this->type = value;
}

inline
Value *dvalue_value(Dvalue *this) {
  return this->value;
}

inline
void dvalue_set_value(Dvalue *this, Value *value) {
  this->value = value;
}

Json *dvalue_serialize(Dvalue *this) {
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, filepos_serialize(this->pos));
  jarr_astring(serial, this->id);
  arr_add(serial, filepos_serialize(this->tpos));
  arr_add(serial, type_serialize(this->type));
  arr_add(serial, value_serialize(this->value));
  return json_warray(serial);
}

Dvalue *dvalue_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Dvalue *this = MALLOC(Dvalue);
  size_t i = 0;
  this->pos = filepos_restore(arr_get(serial, i++));
  this->id = jarr_gstring(serial, i++);
  this->tpos = filepos_restore(arr_get(serial, i++));
  this->type = type_restore(arr_get(serial, i++));
  this->value = value_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

inline
Dvalue *dvalue_new(Filepos *pos, char *id) {
  return _dvalue_new(
    pos, id, filepos_new(filepos_path(pos), 0, 0),
    type_new(UNKNOWN, "", arr_new()),
    value_new_null()
  );
}
