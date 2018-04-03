// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Value.h"
#include "ast/Stat.h"

/*.
-struct: @Value
  pos: Pos *
  vtype: enum Vtype_t
  +type: Type *
  data: void *
*/

static Value *v_null = NULL;

// rq_type return 'values' type or UNKNOWN if 'values' has different types,
// has some NULL or is empty.
static Type *eq_type(Arr/*Value*/ *values) {
  if (!arr_size(values)) {
    return type_new_unknown();
  }
  Type *tp = value_type(arr_get(values, 0));
  if (type_is_unknown(tp)){
    return tp;
  }
  EACH(values, Value, v) {
    if (!type_eq(value_type(v), tp)) {
      return type_new_unknown();
    }
  }_EACH
  return tp;
}

/*.-.*/
struct value_Value {
  Pos *pos;
  enum Vtype_t vtype;
  Type *type;
  void *data;
};

Value *_value_new(
  Pos *pos,
  enum Vtype_t vtype,
  Type *type,
  void *data
) {
  Value *this = MALLOC(Value);
  this->pos = pos;
  this->vtype = vtype;
  this->type = type;
  this->data = data;
  return this;
}

inline
Pos *value_pos(Value *this) {
  return this->pos;
}

inline
enum Vtype_t value_vtype(Value *this) {
  return this->vtype;
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
void *value_data(Value *this) {
  return this->data;
}
/*.-.*/

inline
Value *value_new_null(Pos *pos) {
  return v_null
    ? v_null
    : (v_null = _value_new(pos, VNULL, type_new_unknown(), ""));
}

inline
Value *value_new_bool(Pos *pos, char *value) {
  return _value_new(pos, VBOOL, type_new_bool(), value);
}

inline
Value *value_new_byte(Pos *pos, char *value) {
  return _value_new(pos, VBYTE, type_new_byte(), value);
}

Value *value_new_int(Pos *pos, char *value) {
  return _value_new(pos, VINT, type_new_int(), value);
}

Value *value_new_float(Pos *pos, char *value) {
  return _value_new(pos, VFLOAT, type_new_float(), value);
}

Value *value_new_char(Pos *pos, char *value) {
  return _value_new(pos, VCHAR, type_new_char(), value);
}

Value *value_new_str(Pos *pos, char *value) {
  return _value_new(pos, VSTR, type_new_str(DATA, "Str", arr_new()), value);
}

Value *value_new_str2(Pos *pos, char *value) {
  return _value_new(pos, VSTR2, type_new_str(), value);
}

Value *value_new_arr(Pos *pos, Arr/*Value*/ *values) {
  return _value_new(pos, VARR, type_new_arr(eq_type(values)), values);
}

Value *value_new_map(Pos *pos, Arr/*char*/ *keys, Arr/*Value*/ *values) {
  Arr *data2 = arr_new();
  arr_add(data2, keys);
  arr_add(data2, values);
  return _value_new(pos, VMAP, type_new_map(eq_type(values)), data2);
}

Value *value_new_fn(Pos *pos, Arr/*char*/ *params, Arr/*Statement*/ *stats) {
  Arr/*Type*/ *ps = arr_new();
  REPEAT(arr_size(params) + 1){
    arr_add(ps, type_new_unknown());
  }_REPEAT
  Arr *data2 = arr_new();
  arr_add(data2, params);
  arr_add(data2, stats);
  return _value_new(pos, VFN, type_new_fn(ps), data2);
}

Value *value_new_id(Pos *pos, char *id) {
  return _value_new(pos, VID, type_new_unknown(), id);
}

Value *value_new_fid(Pos *pos, char *id, Arr/*Value*/ *values) {
  Arr *data2 = arr_new();
  arr_add(data2, id);
  arr_add(data2, values);
  return _value_new(pos, VFID, type_new_unknown(), data2);
}

Value *value_new_lunary(Pos *pos, char *operator, Value *value) {
  Arr *data2 = arr_new();
  arr_add(data2, operator);
  arr_add(data2, value);
  return _value_new(pos, VLUNARY, type_new_unknown(), data2);
}

Value *value_new_runary(Pos *pos, char *operator, Value *value) {
  Arr *data2 = arr_new();
  arr_add(data2, operator);
  arr_add(data2, value);
  return _value_new(pos, VRUNARY, type_new_unknown(), data2);
}

Value *value_new_binary(Pos *pos, char *operator, Value *v1, Value *v2) {
  Arr *data2 = arr_new();
  arr_add(data2, operator);
  arr_add(data2, v1);
  arr_add(data2, v2);
  return _value_new(pos, VBINARY, type_new_unknown(), data2);
}

Value *value_new_ternary(Pos *pos, Value *v1, Value *v2, Value *v3) {
  Arr *data2 = arr_new();
  arr_add(data2, v1);
  arr_add(data2, v2);
  arr_add(data2, v3);
  return _value_new(pos, VTERNARY, type_new_unknown(), data2);
}

Value *value_new_with(
  Pos *pos, Value *v, Arr/*Value*/ *conditions, Arr/*Value*/ *values
) {
  Arr *data2 = arr_new();
  arr_add(data2, v);
  arr_add(data2, conditions);
  arr_add(data2, values);
  return _value_new(pos, VWITH, type_new_unknown(), data2);
}

Value *value_new_group(Pos *pos, Value *v1) {
  return _value_new(pos, VGROUP, value_type(v1), v1);
}

Json *value_serialize(Value *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, pos_serialize(this->pos));
  jarr_auint(serial, this->vtype);
  arr_add(serial, type_serialize(this->type));

  Arr/*Json*/ *a;
  switch(this->vtype) {
  case VNULL:
  case VBOOL:
  case VBYTE:
  case VINT:
  case VFLOAT:
  case VCHAR:
  case VSTR:
  case VSTR2:
  case VID:
    jarr_astring(serial, this->data);
    break;
  case VARR:
    jarr_aarray(serial, this->data, (Json *(*)(void *))value_serialize);
    break;
  case VMAP:
    a = arr_new();
    jarr_aarray(a, arr_get(this->data, 0), (Json *(*)(void *))value_serialize);
    jarr_aarray(a, arr_get(this->data, 1), (Json *(*)(void *))value_serialize);
    arr_add(serial, json_warray(a));
    break;
  case VFN:
    a = arr_new();
    jarr_aarray(a, arr_get(this->data, 0), (Json *(*)(void *))json_wstring);
    jarr_aarray(a, arr_get(this->data, 1), (Json *(*)(void *))stat_serialize);
    arr_add(serial, json_warray(a));
    break;
  case VFID:
    a = arr_new();
    jarr_astring(a, arr_get(this->data, 0));
    jarr_aarray(a, arr_get(this->data, 1), (Json *(*)(void *))value_serialize);
    arr_add(serial, json_warray(a));
    break;
  case VLUNARY:
  case VRUNARY:
    a = arr_new();
    jarr_astring(a, arr_get(this->data, 0));
    arr_add(a, value_serialize(arr_get(this->data, 1)));
    arr_add(serial, json_warray(a));
    break;
  case VBINARY:
    a = arr_new();
    jarr_astring(a, arr_get(this->data, 0));
    arr_add(a, value_serialize(arr_get(this->data, 1)));
    arr_add(a, value_serialize(arr_get(this->data, 2)));
    arr_add(serial, json_warray(a));
    break;
  case VTERNARY:
    a = arr_new();
    arr_add(a, value_serialize(arr_get(this->data, 0)));
    arr_add(a, value_serialize(arr_get(this->data, 1)));
    arr_add(a, value_serialize(arr_get(this->data, 2)));
    arr_add(serial, json_warray(a));
    break;
  case VWITH:
    a = arr_new();
    arr_add(a, value_serialize(arr_get(this->data, 0)));
    jarr_aarray(a, arr_get(this->data, 1), (Json *(*)(void *))value_serialize);
    jarr_aarray(a, arr_get(this->data, 2), (Json *(*)(void *))value_serialize);
    arr_add(serial, json_warray(a));
    break;
  case VGROUP:
    arr_add(serial, value_serialize(this->data));
    break;
  default:
    THROW "Unkown this->vtype" _THROW
  }

  return json_warray(serial);
}

Value *value_restore(Json *s) {
  if (json_rnull(s)) return NULL;
  Arr/*Json*/ *serial = json_rarray(s);
  Value *this = MALLOC(Value);
  size_t i = 0;
  this->pos = pos_restore(arr_get(serial, i++));
  this->vtype = jarr_guint(serial, i++);
  this->type = type_restore(arr_get(serial, i++));

  Arr/*Json*/ *a2;
  Arr *a;
  switch(this->vtype) {
  case VNULL:
  case VBOOL:
  case VBYTE:
  case VINT:
  case VFLOAT:
  case VCHAR:
  case VSTR:
  case VSTR2:
  case VID:
    this->data = jarr_gstring(serial, i);
    break;
  case VARR:
    this->data = jarr_garray(serial, i, (void *(*)(Json *))value_restore);
    break;
  case VMAP:
    a2 = json_rarray(arr_get(serial, i));
    a = arr_new();
    arr_add(a, jarr_garray(a2, 0, (void *(*)(Json *))value_restore));
    arr_add(a, jarr_garray(a2, 1, (void *(*)(Json *))value_restore));
    this->data = a;
    break;
  case VFN:
    a2 = json_rarray(arr_get(serial, i));
    a = arr_new();
    arr_add(a, jarr_garray(a2, 0, (void *(*)(Json *))json_rstring));
    arr_add(a, jarr_garray(a2, 1, (void *(*)(Json *))stat_restore));
    this->data = a;
    break;
  case VFID:
    a2 = json_rarray(arr_get(serial, i));
    a = arr_new();
    arr_add(a, jarr_gstring(a2, 0));
    arr_add(a, jarr_garray(a2, 1, (void *(*)(Json *))value_restore));
    this->data = a;
    break;
  case VLUNARY:
  case VRUNARY:
    a2 = json_rarray(arr_get(serial, i));
    a = arr_new();
    arr_add(a, jarr_gstring(a2, 0));
    arr_add(a, value_restore(arr_get(a2, 1)));
    this->data = a;
    break;
  case VBINARY:
    a2 = json_rarray(arr_get(serial, i));
    a = arr_new();
    arr_add(a, jarr_gstring(a2, 0));
    arr_add(a, value_restore(arr_get(a2, 1)));
    arr_add(a, value_restore(arr_get(a2, 2)));
    this->data = a;
    break;
  case VTERNARY:
    a2 = json_rarray(arr_get(serial, i));
    a = arr_new();
    arr_add(a, value_restore(arr_get(a2, 0)));
    arr_add(a, value_restore(arr_get(a2, 1)));
    arr_add(a, value_restore(arr_get(a2, 2)));
    this->data = a;
    break;
  case VWITH:
    a2 = json_rarray(arr_get(serial, i));
    a = arr_new();
    arr_add(a, value_restore(arr_get(a2, 0)));
    arr_add(a, jarr_garray(a2, 1, (void *(*)(Json *))value_restore));
    arr_add(a, jarr_garray(a2, 2, (void *(*)(Json *))value_restore));
    this->data = a;
    break;
  case VGROUP:
    this->data = value_restore(arr_get(serial, i));
    break;
  default:
    THROW "Unkown this->vtype" _THROW
  }

  return this;
}
