// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Value.h"
#include "ast/Atype.h"
#include "ast/Avalue.h"
#include "ast/Astat.h"

/*.
struct: Value
  vtype: enum Value_t : _uint
  pos: Pos *: pos
  type: Type *: type
  attachs: Avatt *: avatt
  data: char *: _string
*/
/*.-.*/
struct value_Value {
  enum Value_t vtype;
  Pos *pos;
  Type *type;
  Avatt *attachs;
  char *data;
};

Value *value_new(
  enum Value_t vtype,
  Pos *pos,
  Type *type,
  Avatt *attachs,
  char *data
) {
  Value *this = MALLOC(Value);
  this->vtype = vtype;
  this->pos = pos;
  this->type = type;
  this->attachs = attachs;
  this->data = data;
  return this;
}

inline
enum Value_t value_vtype(Value *this) {
  return this->vtype;
}

inline
Pos *value_pos(Value *this) {
  return this->pos;
}

inline
Type *value_type(Value *this) {
  return this->type;
}

inline
Avatt *value_attachs(Value *this) {
  return this->attachs;
}

inline
char *value_data(Value *this) {
  return this->data;
}

Json *value_serialize(Value *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->vtype);
  arr_add(serial, pos_serialize(this->pos));
  arr_add(serial, type_serialize(this->type));
  arr_add(serial, avatt_serialize(this->attachs));
  jarr_astring(serial, this->data);
  return json_warray(serial);
}

Value *value_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Value *this = MALLOC(Value);
  size_t i = 0;
  this->vtype = jarr_guint(serial, i++);
  this->pos = pos_restore(arr_get(serial, i++));
  this->type = type_restore(arr_get(serial, i++));
  this->attachs = avatt_restore(arr_get(serial, i++));
  this->data = jarr_gstring(serial, i++);
  return this;
}
/*.-.*/

inline
Value *value_new_null(Pos *pos) {
  return value_new(VNULL, pos, type_new_unknown(), avatt_new(), "");
}

inline
Value *value_new_bool(Pos *pos, Avatt *atts, char *value) {
  return value_new(VBOOL, pos, type_new_bool(), atts, value);
}

inline
Value *value_new_byte(Pos *pos, char *value) {
  return value_new(VBYTE, pos, type_new_byte(), avatt_new(), value);
}

inline
Value *value_new_int(Pos *pos, char *value) {
  return value_new(VINT, pos, type_new_int(), avatt_new(), value);
}

inline
Value *value_new_float(Pos *pos, char *value) {
  return value_new(VFLOAT, pos, type_new_float(), avatt_new(), value);
}

inline
Value *value_new_char(Pos *pos, Avatt *atts, char *value) {
  return value_new(VCHAR, pos, type_new_char(), atts, value);
}

inline
Value *value_new_str(Pos *pos, Avatt *atts, char *value) {
  return value_new(VSTR, pos, type_new_str(), atts, value);
}

inline
Value *value_new_str2(Pos *pos, Avatt *atts, char *value) {
  return value_new(VSTR2, pos, type_new_str(), atts, value);
}

static Type *common_type(Atype *types) {
  Type *r = atype_get(types, 0);
  EACH(types, Type, t) {
    if (!type_eq(t, r)) {
      return type_new_unknown();
    }
  }_EACH
  return r;
}

static Type *ct_value(Avalue *vs) {
  Atype *ts = atype_new();
  EACH(vs, Value, v) {
    atype_add(ts, v->type);
  }_EACH
  return common_type(ts);
}

inline
Value *value_new_arr(Pos *pos, Avatt *atts, Avalue *values) {
  return value_new(
    VARR, pos, type_new_arr(ct_value(values)), atts, avalue_serialize(values)
  );
}

static Type *ct_value2(Map/*Value*/ *m) {
  Atype *ts = atype_new();
  Value *v;
  EACH(m, Kv, kv) {
    v = kv->value;
    atype_add(ts, v->type);
  }_EACH
  return common_type(ts);
}

Value *value_new_map(Pos *pos, Avatt *atts, Map/*Value*/ *m) {
  Map/*Json*/ *jm = map_new();
  EACH(m, Kv, kv) {
    arr_add(jm, kv_new(kv->key, value_serialize(kv->value)));
  }_EACH
  return value_new(
    VMAP, pos, type_new_arr(ct_value2(m)), atts, json_wobject(jm)
  );
}

Value *value_new_fn(Pos *pos, Achar *params, Astat *stats) {
  Arr/*Json*/ *js = arr_new();
  arr_add(js, achar_serialize(params));
  arr_add(js, astat_serialize(stats));
  return value_new(
    VFN, pos, type_new_unknown(), avatt_new(), json_warray(js)
  );
}

inline
Value *value_new_id(Pos *pos, Avatt *atts, char *id) {
  return value_new(VID, pos, type_new_unknown(), atts, id);
}

inline
Value *value_new_new(Pos *pos, Type *tp, Avalue *values) {
  return value_new(VNEW, pos, tp, avatt_new(), avalue_serialize(values));
}

inline
Value *value_new_group(Pos *pos, Avatt *atts, Value *v1) {
  return value_new(VGROUP, pos, v1->type, atts, value_serialize(v1));
}
