// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Value.h"
#include "ast/Stat.h"

/*.
-struct: @Value
  vtype: enum Vtype_t
  type: Type *
  data: void *
*/

/*.-.*/
struct value_Value {
  enum Vtype_t vtype;
  Type *type;
  void *data;
};

Value *_value_new(enum Vtype_t vtype, Type *type, void *data) {
  Value *this = MALLOC(Value);
  this->vtype = vtype;
  this->type = type;
  this->data = data;
  return this;
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
void *value_data(Value *this) {
  return this->data;
}
/*.-.*/

inline
Value *value_new_null(void) {
  return _value_new(VNULL, type_new(UNKNOWN, "", arr_new()), "");
}

inline
Value *value_new_bool(char *value) {
  return _value_new(VBOOL, type_new(DATA, "Bool", arr_new()), value);
}

inline
Value *value_new_byte(char *value) {
  return _value_new(VBYTE, type_new(DATA, "Byte", arr_new()), value);
}

Value *value_new_int(char *value) {
  return _value_new(VINT, type_new(DATA, "Int", arr_new()), value);
}

Value *value_new_float(char *value) {
  return _value_new(VFLOAT, type_new(DATA, "Float", arr_new()), value);
}

Value *value_new_char(char *value) {
  return _value_new(VCHAR, type_new(DATA, "Char", arr_new()), value);
}

Value *value_new_str(char *value) {
  return _value_new(VSTR, type_new(DATA, "Str", arr_new()), value);
}

Value *value_new_arr(Arr/*Value*/ *values) {
  Arr/*Type*/ *ps = arr_new();
  arr_add(ps, type_new(UNKNOWN, "", arr_new()));
  return _value_new(VARR, type_new(ARR, "", ps), values);
}

Value *value_new_map(Arr/*char*/ *keys, Arr/*Value*/ *values) {
  Arr/*Type*/ *ps = arr_new();
  arr_add(ps, type_new(UNKNOWN, "", arr_new()));
  Arr *data2 = arr_new();
  arr_add(data2, keys);
  arr_add(data2, values);
  return _value_new(VMAP, type_new(MAP, "", ps), data2);
}

Value *value_new_fn(Arr/*char*/ *params, Arr/*Statement*/ *values) {
  Arr/*Type*/ *ps = arr_new();
  REPEAT(arr_size(params)){
    arr_add(ps, type_new(UNKNOWN, "", arr_new()));
  }_REPEAT
  Arr *data2 = arr_new();
  arr_add(data2, params);
  arr_add(data2, values);
  return _value_new(VFN, type_new(FN, "", ps), data2);
}

Value *value_new_id(char *id) {
  return _value_new(VID, type_new(UNKNOWN, "", arr_new()), id);
}

Value *value_new_fid(char *id, Arr/*Value*/ *values) {
  Arr *data2 = arr_new();
  arr_add(data2, id);
  arr_add(data2, values);
  return _value_new(VFID, type_new(UNKNOWN, "", arr_new()), data2);
}

Value *value_new_lmonadic(char *operator, Value *value) {
  Arr *data2 = arr_new();
  arr_add(data2, operator);
  arr_add(data2, value);
  return _value_new(VLMONADIC, type_new(UNKNOWN, "", arr_new()), data2);
}

Value *value_new_rmonadic(char *operator, Value *value) {
  Arr *data2 = arr_new();
  arr_add(data2, operator);
  arr_add(data2, value);
  return _value_new(VLMONADIC, type_new(UNKNOWN, "", arr_new()), data2);
}

Value *value_new_binary(char *operator, Value *v1, Value *v2) {
  Arr *data2 = arr_new();
  arr_add(data2, operator);
  arr_add(data2, v1);
  arr_add(data2, v2);
  return _value_new(VLMONADIC, type_new(UNKNOWN, "", arr_new()), data2);
}

Value *value_new_ternary(char *operator, Value *v1, Value *v2, Value *v3) {
  Arr *data2 = arr_new();
  arr_add(data2, operator);
  arr_add(data2, v1);
  arr_add(data2, v2);
  arr_add(data2, v3);
  return _value_new(VLMONADIC, type_new(UNKNOWN, "", arr_new()), data2);
}

Json *value_serialize(Value *this) {
  Arr/*Json*/ *serial = arr_new();
  jarr_auint(serial, this->vtype);
  arr_add(serial, type_serialize(this->type));

  switch(this->vtype) {
  case VNULL:
  case VBOOL:
  case VBYTE:
  case VINT:
  case VFLOAT:
  case VCHAR:
  case VSTR:
  case VID:
    jarr_astring(serial, this->data);
    break;
  case VARR:
    jarr_aarray(serial, this->data, (Json *(*)(void *))value_serialize);
    break;
  case VMAP: {
    Arr/*Json*/ *a = arr_new();
    jarr_aarray(a, arr_get(this->data, 0), (Json *(*)(void *))json_wstring);
    jarr_aarray(a, arr_get(this->data, 1), (Json *(*)(void *))value_serialize);
    arr_add(serial, json_warray(a));
    break;
  }
  case VFN: {
    Arr/*Json*/ *a = arr_new();
    jarr_aarray(a, arr_get(this->data, 0), (Json *(*)(void *))json_wstring);
    jarr_aarray(a, arr_get(this->data, 1), (Json *(*)(void *))stat_serialize);
    arr_add(serial, json_warray(a));
    break;
  }
  case VFID: {
    Arr/*Json*/ *a = arr_new();
    jarr_astring(a, arr_get(this->data, 0));
    jarr_aarray(a, arr_get(this->data, 1), (Json *(*)(void *))value_serialize);
    arr_add(serial, json_warray(a));
    break;
  }
  case VLMONADIC:
  case VRMONADIC: {
    Arr/*Json*/ *a = arr_new();
    jarr_astring(a, arr_get(this->data, 0));
    arr_add(a, value_serialize(arr_get(this->data, 1)));
    arr_add(serial, json_warray(a));
    break;
  }
  case VBINARY: {
    Arr/*Json*/ *a = arr_new();
    jarr_astring(a, arr_get(this->data, 0));
    arr_add(a, value_serialize(arr_get(this->data, 1)));
    arr_add(a, value_serialize(arr_get(this->data, 2)));
    arr_add(serial, json_warray(a));
    break;
  }
  case VTERNARY: {
    Arr/*Json*/ *a = arr_new();
    jarr_astring(a, arr_get(this->data, 0));
    arr_add(a, value_serialize(arr_get(this->data, 1)));
    arr_add(a, value_serialize(arr_get(this->data, 2)));
    arr_add(a, value_serialize(arr_get(this->data, 3)));
    arr_add(serial, json_warray(a));
    break;
  }
  default:
    THROW "Unkown this->vtype" _THROW
  }

  return json_warray(serial);
}

Value *value_restore(Json *s) {
  Arr/*Json*/ *serial = json_rarray(s);
  Value *this = MALLOC(Value);
  size_t i = 0;
  this->vtype = jarr_guint(serial, i++);
  this->type = type_restore(arr_get(serial, i++));

  switch(this->vtype) {
  case VNULL:
  case VBOOL:
  case VBYTE:
  case VINT:
  case VFLOAT:
  case VCHAR:
  case VSTR:
  case VID:
    this->data = jarr_gstring(serial, i);
    break;
  case VARR:
    this->data = jarr_garray(serial, i, (void *(*)(Json *))value_restore);
    break;
  case VMAP: {
    Arr/*Json*/ *a2 = json_rarray(arr_get(serial, i));
    Arr *a = arr_new();
    arr_add(a, jarr_garray(a2, 0, (void *(*)(Json *))json_rstring));
    arr_add(a, jarr_garray(a2, 1, (void *(*)(Json *))value_restore));
    this->data = a;
    break;
  }
  case VFN: {
    Arr/*Json*/ *a2 = json_rarray(arr_get(serial, i));
    Arr *a = arr_new();
    arr_add(a, jarr_garray(a2, 0, (void *(*)(Json *))json_rstring));
    arr_add(a, jarr_garray(a2, 1, (void *(*)(Json *))stat_restore));
    this->data = a;
    break;
  }
  case VFID: {
    Arr/*Json*/ *a2 = json_rarray(arr_get(serial, i));
    Arr *a = arr_new();
    arr_add(a, jarr_gstring(a2, 0));
    arr_add(a, jarr_garray(a2, 1, (void *(*)(Json *))value_restore));
    this->data = a;
    break;
  }
  case VLMONADIC:
  case VRMONADIC: {
    Arr/*Json*/ *a2 = json_rarray(arr_get(serial, i));
    Arr *a = arr_new();
    arr_add(a, jarr_gstring(a2, 0));
    arr_add(a, value_restore(arr_get(a2, 1)));
    this->data = a;
    break;
  }
  case VBINARY: {
    Arr/*Json*/ *a2 = json_rarray(arr_get(serial, i));
    Arr *a = arr_new();
    arr_add(a, jarr_gstring(a2, 0));
    arr_add(a, value_restore(arr_get(a2, 1)));
    arr_add(a, value_restore(arr_get(a2, 2)));
    this->data = a;
    break;
  }
  case VTERNARY: {
    Arr/*Json*/ *a2 = json_rarray(arr_get(serial, i));
    Arr *a = arr_new();
    arr_add(a, jarr_gstring(a2, 0));
    arr_add(a, value_restore(arr_get(a2, 1)));
    arr_add(a, value_restore(arr_get(a2, 2)));
    arr_add(a, value_restore(arr_get(a2, 3)));
    this->data = a;
    break;
  }
  default:
    THROW "Unkown this->vtype" _THROW
  }

  return this;
}

bool value_eq(Value *this, Value *other) {
  if (this->vtype != other->vtype) {
    return false;
  }
  if (!type_eq(this->type, other->type)) {
    return false;
  }

  switch(this->vtype) {
  case VNULL:
  case VBOOL:
  case VBYTE:
  case VINT:
  case VFLOAT:
  case VCHAR:
  case VSTR:
  case VID:
    if (strcmp(this->data, other->data)) return false;
    break;
  case VARR:
    if (
      !it_eq(
        it_from(this->data),
        it_from(other->data),
        (bool (*)(void *, void *))value_eq
      )
    ) return false;
    break;
  case VMAP:
    if (
      !it_eq_str(
        it_from(arr_get(this->data, 0)),
        it_from(arr_get(other->data, 0))
      ) ||
      !it_eq(
        it_from(arr_get(this->data, 1)),
        it_from(arr_get(other->data, 1)),
        (bool (*)(void *, void *))value_eq
      )
    ) return false;
    break;
  case VFN:
    if (
      !it_eq_str(
        it_from(arr_get(this->data, 0)),
        it_from(arr_get(other->data, 0))
      ) ||
      !it_eq(
        it_from(arr_get(this->data, 1)),
        it_from(arr_get(other->data, 1)),
        (bool (*)(void *, void *))stat_eq
      )
    ) return false;
    break;
  case VFID:
    if (
      strcmp(arr_get(this->data, 0), arr_get(other->data, 0)) ||
      !it_eq(
        it_from(arr_get(this->data, 1)),
        it_from(arr_get(other->data, 1)),
        (bool (*)(void *, void *))value_eq
      )
    ) return false;
    break;
  case VLMONADIC:
  case VRMONADIC: {
    if (
      strcmp(arr_get(this->data, 0), arr_get(other->data, 0)) ||
      !value_eq(arr_get(this->data, 1), arr_get(other->data, 1))
    ) return false;
    break;
  }
  case VBINARY: {
    if (
      strcmp(arr_get(this->data, 0), arr_get(other->data, 0)) ||
      !value_eq(arr_get(this->data, 1), arr_get(other->data, 1)) ||
      !value_eq(arr_get(this->data, 1), arr_get(other->data, 2))
    ) return false;
    break;
  }
  case VTERNARY: {
    if (
      strcmp(arr_get(this->data, 0), arr_get(other->data, 0)) ||
      !value_eq(arr_get(this->data, 1), arr_get(other->data, 1)) ||
      !value_eq(arr_get(this->data, 1), arr_get(other->data, 2)) ||
      !value_eq(arr_get(this->data, 1), arr_get(other->data, 3))
    ) return false;
    break;
  }
  default:
    THROW "Unkown this->vtype" _THROW
  }

  return true;
}

