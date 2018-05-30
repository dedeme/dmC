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
  +type: Type *: type
  attachs: Avatt *: avatt
  data: Achar *: achar
*/
/*.-.*/
struct value_Value {
  enum Value_t vtype;
  Pos *pos;
  Type *type;
  Avatt *attachs;
  Achar *data;
};

Value *value_new(
  enum Value_t vtype,
  Pos *pos,
  Type *type,
  Avatt *attachs,
  Achar *data
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
void value_set_type(Value *this, Type *value) {
  this->type = value;
}

inline
Avatt *value_attachs(Value *this) {
  return this->attachs;
}

inline
Achar *value_data(Value *this) {
  return this->data;
}

Arr/*Json*/ *value_serialize(Value *this) {
  Arr/*Json*/ *serial = arr_new();
  if (!this) return serial;
  jarr_auint(serial, this->vtype);
  arr_add(serial, json_warray(pos_serialize(this->pos)));
  arr_add(serial, json_warray(type_serialize(this->type)));
  arr_add(serial, json_warray(avatt_serialize(this->attachs)));
  arr_add(serial, json_warray(achar_serialize(this->data)));
  return serial;
}

Value *value_restore(Arr/*Json*/ *serial) {
  if (!arr_size(serial)) return NULL;
  Value *this = MALLOC(Value);
  size_t i = 0;
  this->vtype = jarr_guint(serial, i++);
  this->pos = pos_restore(json_rarray(arr_get(serial, i++)));
  this->type = type_restore(json_rarray(arr_get(serial, i++)));
  this->attachs = avatt_restore(json_rarray(arr_get(serial, i++)));
  this->data = achar_restore(json_rarray(arr_get(serial, i++)));
  return this;
}
/*.-.*/

Achar *mk_achar(char *value) {
  Achar *r = achar_new();
  achar_add(r, value);
  return r;
}

inline
Value *value_new_null(Pos *pos) {
  return value_new(VNULL, pos, type_new_unknown(), avatt_new(), mk_achar(""));
}

inline
Value *value_new_bool(Pos *pos, Avatt *atts, char *value) {
  return value_new(VBOOL, pos, type_new_bool(), atts, mk_achar(value));
}

inline
Value *value_new_byte(Pos *pos, char *value) {
  return value_new(VBYTE, pos, type_new_byte(), avatt_new(), mk_achar(value));
}

inline
Value *value_new_int(Pos *pos, char *value) {
  return value_new(VINT, pos, type_new_int(), avatt_new(), mk_achar(value));
}

inline
Value *value_new_float(Pos *pos, char *value) {
  return value_new(VFLOAT, pos, type_new_float(), avatt_new(), mk_achar(value));
}

inline
Value *value_new_char(Pos *pos, Avatt *atts, char *value) {
  return value_new(VCHAR, pos, type_new_char(), atts, mk_achar(value));
}

inline
Value *value_new_str(Pos *pos, Avatt *atts, char *value) {
  return value_new(VSTR, pos, type_new_str(), atts, mk_achar(value));
}

inline
Value *value_new_str2(Pos *pos, Avatt *atts, char *value) {
  return value_new(VSTR2, pos, type_new_str(), atts, mk_achar(value));
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
  if (!arr_size(vs)) {
    return type_new_unknown();
  }
  Atype *ts = atype_new();
  EACH(vs, Value, v) {
    atype_add(ts, v->type);
  }_EACH
  return common_type(ts);
}

inline
Value *value_new_arr(Pos *pos, Avatt *atts, Avalue *values) {
  return value_new(
    VARR, pos, type_new_arr(ct_value(values)), atts,
    (Achar *)avalue_serialize(values)
  );
}

static Type *ct_value2(Map/*Value*/ *m) {
  if (!arr_size(m)) {
    return type_new_unknown();
  }
  Atype *ts = atype_new();
  bool is_key = true;
  EACH(m, Value, v) {
    if (is_key) {
      is_key = false;
    } else {
      atype_add(ts, v->type);
      is_key = true;
    }
  }_EACH
  return common_type(ts);
}

Value *value_new_map(Pos *pos, Avatt *atts, Arr/*Value*/ *m) {
  return value_new(
    VMAP, pos, type_new_map(ct_value2(m)), atts, (Achar *)avalue_serialize(m)
  );
}

Value *value_new_fn(Pos *pos, Achar *params, Astat *stats) {
  Arr/*Json*/ *js = arr_new();
  arr_add(js, achar_serialize(params));
  arr_add(js, astat_serialize(stats));
  return value_new(
    VFN, pos, type_new_unknown(), avatt_new(), (Achar *)js
  );
}

inline
Value *value_new_id(Pos *pos, Avatt *atts, char *id, Achar *generics) {
  achar_add(generics, id);
  return value_new(VID, pos, type_new_unknown(), atts, generics);
}

inline
Value *value_new_group(Pos *pos, Avatt *atts, Value *v) {
  return value_new(VGROUP, pos, v->type, atts, (Achar *)value_serialize(v));
}

inline
Value *value_new_cast(Pos *pos, Type *tp, Value *v) {
  return value_new(VCAST, pos, tp, avatt_new(), (Achar *)value_serialize(v));
}

Value *value_new_lunary(Pos *pos, char *op, Value *v) {
  Achar *data = (Achar *)value_serialize(v);
  achar_add(data, op);
  return value_new(
    VLUNARY, pos, value_type(v), avatt_new(), data
  );
}

Value *value_new_runary(Pos *pos, char *op, Value *v) {
  Achar *data = (Achar *)value_serialize(v);
  achar_add(data, op);
  return value_new(
    VRUNARY, pos, value_type(v), avatt_new(), data
  );
}

Value *value_new_binary(Pos *pos, Type *tp, char *op, Value *v1, Value *v2) {
  Avalue *vs = avalue_new();
  avalue_add(vs, v1);
  avalue_add(vs, v2);
  Achar *data = (Achar *)avalue_serialize(vs);
  achar_add(data, op);
  return value_new(VBINARY, pos, tp, avatt_new(), data);
}

Value *value_new_ternary(Pos *pos, Type *tp, Value *v1, Value *v2, Value *v3) {
  Avalue *vs = avalue_new();
  avalue_add(vs, v1);
  avalue_add(vs, v2);
  avalue_add(vs, v3);
  return value_new(
    VTERNARY, pos, tp, avatt_new(), (Achar *)avalue_serialize(vs)
  );
}
