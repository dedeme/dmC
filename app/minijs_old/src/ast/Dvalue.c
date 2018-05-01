// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Dvalue.h"

/*.
struct: @Dvalue
  pos: Pos *: pos
  id: char *: _string
  +tpos: Pos *: pos
  +type: Type *: type
  +value: Value *: value
*/

/*.-.*/
struct dvalue_Dvalue {
  Pos *pos;
  char *id;
  Pos *tpos;
  Type *type;
  Value *value;
};

Dvalue *_dvalue_new(
  Pos *pos,
  char *id,
  Pos *tpos,
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
Pos *dvalue_pos(Dvalue *this) {
  return this->pos;
}

inline
char *dvalue_id(Dvalue *this) {
  return this->id;
}

inline
Pos *dvalue_tpos(Dvalue *this) {
  return this->tpos;
}

inline
void dvalue_set_tpos(Dvalue *this, Pos *value) {
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
  arr_add(serial, pos_serialize(this->pos));
  jarr_astring(serial, this->id);
  arr_add(serial, pos_serialize(this->tpos));
  arr_add(serial, type_serialize(this->type));
  arr_add(serial, value_serialize(this->value));
  return json_warray(serial);
}

Dvalue *dvalue_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Dvalue *this = MALLOC(Dvalue);
  size_t i = 0;
  this->pos = pos_restore(arr_get(serial, i++));
  this->id = jarr_gstring(serial, i++);
  this->tpos = pos_restore(arr_get(serial, i++));
  this->type = type_restore(arr_get(serial, i++));
  this->value = value_restore(arr_get(serial, i++));
  return this;
}
/*.-.*/

inline
Dvalue *dvalue_new(Pos *pos, char *id) {
  return _dvalue_new(
    pos, id, pos_new(0, 0), type_new_unknown(), value_new_null(pos)
  );
}
