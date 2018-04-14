// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "ast/Value.h"
#include "ast/Attach.h"
#include "ast/Stat.h"

/*.
struct: @Value
  pos: Pos *: pos
  vtype: enum Vtype_t: _uint
  +type: Type *: type
  ids: Arr *: _sarray
  values: Arr *: _array value
  attachs: Arr *: _array attach
  stats: Arr *: _array stat
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

// rq_type return 'values' type or UNKNOWN if 'values' has different types,
// has some NULL or is empty.
static Type *eq_type_map(Arr/*Value*/ *values) {
  if (!arr_size(values)) {
    return type_new_unknown();
  }
  Type *tp = value_type(arr_get(values, 1));
  if (type_is_unknown(tp)){
    return tp;
  }
  bool is_key = true;
  EACH(values, Value, v) {
    if (is_key) {
      is_key = false;
      continue;
    }
    is_key = true;
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
  Arr *ids;
  Arr *values;
  Arr *attachs;
  Arr *stats;
};

Value *_value_new(
  Pos *pos,
  enum Vtype_t vtype,
  Type *type,
  Arr *ids,
  Arr *values,
  Arr *attachs,
  Arr *stats
) {
  Value *this = MALLOC(Value);
  this->pos = pos;
  this->vtype = vtype;
  this->type = type;
  this->ids = ids;
  this->values = values;
  this->attachs = attachs;
  this->stats = stats;
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
Arr *value_ids(Value *this) {
  return this->ids;
}

inline
Arr *value_values(Value *this) {
  return this->values;
}

inline
Arr *value_attachs(Value *this) {
  return this->attachs;
}

inline
Arr *value_stats(Value *this) {
  return this->stats;
}

Json *value_serialize(Value *this) {
  if (!this) return json_wnull();
  Arr/*Json*/ *serial = arr_new();
  arr_add(serial, pos_serialize(this->pos));
  jarr_auint(serial, this->vtype);
  arr_add(serial, type_serialize(this->type));
  jarr_aarray(serial, this->ids, (Json*(*)(void*))json_wstring);
  jarr_aarray(serial, this->values, (Json*(*)(void*)) value_serialize);
  jarr_aarray(serial, this->attachs, (Json*(*)(void*)) attach_serialize);
  jarr_aarray(serial, this->stats, (Json*(*)(void*)) stat_serialize);
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
  this->ids = jarr_garray(serial, i++, (void*(*)(Json*))json_rstring);
  this->values = jarr_garray(serial, i++, (void*(*)(Json*)) value_restore);
  this->attachs = jarr_garray(serial, i++, (void*(*)(Json*)) attach_restore);
  this->stats = jarr_garray(serial, i++, (void*(*)(Json*)) stat_restore);
  return this;
}
/*.-.*/

static Arr/*char*/ *to_arr(char *v) {
  Arr/*char*/ *a = arr_new();
  arr_add(a, v);
  return a;
}

inline
Value *value_new_null(Pos *pos) {
  return v_null
    ? v_null
    : (v_null = _value_new(
        pos, VNULL, type_new_unknown(),
        arr_new(), arr_new(), arr_new(), arr_new()
      ));
}

inline
Value *value_new_bool(Pos *pos, Arr/*Attach*/ *atts, char *value) {
  return _value_new(
    pos, VBOOL, arr_size(atts) ? type_new_unknown() : type_new_bool(),
    to_arr(value), arr_new(), atts, arr_new()
  );
}

inline
Value *value_new_byte(Pos *pos, Arr/*Attach*/ *atts, char *value) {
  return _value_new(
    pos, VBYTE, arr_size(atts) ? type_new_unknown() : type_new_byte(),
    to_arr(value), arr_new(), atts, arr_new()
  );
}

Value *value_new_int(Pos *pos, char *value) {
  return _value_new(
    pos, VINT, type_new_int(),
    to_arr(value), arr_new(), arr_new(), arr_new()
  );
}

Value *value_new_float(Pos *pos, char *value) {
  return _value_new(
    pos, VFLOAT, type_new_float(),
    to_arr(value), arr_new(), arr_new(), arr_new()
  );
}

Value *value_new_char(Pos *pos, Arr/*Attach*/ *atts, char *value) {
  return _value_new(
    pos, VCHAR, arr_size(atts) ? type_new_unknown() : type_new_char(),
    to_arr(value), arr_new(), atts, arr_new()
  );
}

Value *value_new_str(Pos *pos, Arr/*Attach*/ *atts, char *value) {
  return _value_new(
    pos, VSTR, arr_size(atts)
      ? arr_size(atts) == 1 && attach_type(arr_get(atts, 0)) == ASUB
        ? type_new_char()
        : type_new_unknown()
      : type_new_str(),
    to_arr(value), arr_new(), atts, arr_new()
  );
}

Value *value_new_str2(Pos *pos, Arr/*Attach*/ *atts, char *value) {
  return _value_new(
    pos, VSTR2, arr_size(atts)
      ? arr_size(atts) == 1 && attach_type(arr_get(atts, 0)) == ASUB
        ? type_new_char()
        : type_new_unknown()
      : type_new_str(),
    to_arr(value), arr_new(), atts, arr_new()
  );
}

Value *value_new_arr(Pos *pos, Arr/*Attach*/ *atts, Arr/*Value*/ *values) {
  return _value_new(
    pos, VARR, arr_size(atts)
      ? arr_size(atts) == 1 && attach_type(arr_get(atts, 0)) == ASUB
        ? eq_type(values)
        : type_new_unknown()
      : type_new_arr(eq_type(values)),
    arr_new(), values, atts, arr_new()
  );
}

Value *value_new_map(Pos *pos, Arr/*Attach*/ *atts, Arr/*Value*/ *values) {
  return _value_new(
    pos, VMAP,  arr_size(atts)
      ? arr_size(atts) == 1 && attach_type(arr_get(atts, 0)) == ASUB
        ? eq_type_map(values)
        : type_new_unknown()
      : type_new_arr(eq_type_map(values)),
    arr_new(), values, atts, arr_new()
  );
}

Value *value_new_fn(Pos *pos, Arr/*char*/ *params, Arr/*Statement*/ *stats) {
  Arr/*Type*/ *ps = arr_new();
  REPEAT(arr_size(params) + 1){
    arr_add(ps, type_new_unknown());
  }_REPEAT
  return _value_new(
    pos, VFN, type_new_fn(ps),
    params, arr_new(), arr_new(), stats
  );
}

Value *value_new_id(Pos *pos, Arr/*Attach*/ *atts, char *id) {
  return _value_new(
    pos, VID, type_new_unknown(),
    to_arr(id), arr_new(), atts, arr_new()
  );
}

Value *value_new_lunary(Pos *pos, char *operator, Value *value) {
  Arr/*Value*/ *values = arr_new();
  arr_add(values, value);
  return _value_new(
    pos, VLUNARY, type_new_unknown(),
    to_arr(operator), values, arr_new(), arr_new()
  );
}

Value *value_new_runary(Pos *pos, char *operator, Value *value) {
  Arr/*Value*/ *values = arr_new();
  arr_add(values, value);
  return _value_new(
    pos, VRUNARY, type_new_unknown(),
    to_arr(operator), values, arr_new(), arr_new()
  );
}

Value *value_new_binary(Pos *pos, char *operator, Value *v1, Value *v2) {
  Arr/*Value*/ *values = arr_new();
  arr_add(values, v1);
  arr_add(values, v2);
  return _value_new(
    pos, VBINARY, type_new_unknown(),
    to_arr(operator), values, arr_new(), arr_new()
  );
}

Value *value_new_ternary(Pos *pos, Value *v1, Value *v2, Value *v3) {
  Arr/*Value*/ *values = arr_new();
  arr_add(values, v1);
  arr_add(values, v2);
  arr_add(values, v3);
  return _value_new(
    pos, VTERNARY, type_new_unknown(),
    arr_new(), values, arr_new(), arr_new()
  );
}

Value *value_new_with(
  Pos *pos, Value *v, Arr/*Value*/ *conditions, Arr/*Value*/ *values
) {
  Arr/*Value*/ *vs = it_to(
    it_cat(it_unary(v), it_cat(it_from(conditions), it_from(values)))
  );
  return _value_new(
    pos, VWITH, type_new_unknown(),
    arr_new(), vs, arr_new(), arr_new()
  );
}

Value *value_new_new(Pos *pos, Type *tp, Arr/*Value*/ *values) {
  char *id = type_to_str(tp);
  char ix = str_cindex(id, '<');
  if (ix != -1) {
    id = str_sub(id, 0, ix);
  }
  Arr/*char*/ *ids = arr_new();
  arr_add(ids, id);
  return _value_new(
    pos, VNEW, tp,
    ids, values, arr_new(), arr_new()
  );
}

Value *value_new_group(Pos *pos, Arr/*Attach*/ *atts, Value *v1) {
  Arr/*Value*/ *values = arr_new();
  arr_add(values, v1);
  return _value_new(
    pos, VGROUP, arr_size(atts) ? type_new_unknown() : value_type(v1),
    arr_new(), values, atts, arr_new()
  );
}
