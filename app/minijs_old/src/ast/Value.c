// Copyright 10-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

#include <gc.h>
#include "dmc/ct/Achar.h"
#include "dmc/str.h"
#include "dmc/exc.h"
#include "dmc/DEFS.h"
#include "ct/Avatt.h"
#include "ct/Avalue.h"
#include "ct/Astat.h"
#include "ct/Ltype.h"
#include "ast/Value.h"
#include "ast/Vatt.h"
#include "ast/Type.h"
#include "ast/Stat.h"
#include "lexer/Pos.h"

/*.
struct: @Value
  t: enum Value_t: _int
  pos: Pos *: pos
  type: Type *: type
  attachs: Avatt *: __avatt - vatt
  data: Achar *: __achar - str
*/

/*.-.*/
struct value_Value {
  enum Value_t t;
  Pos *pos;
  Type *type;
  Avatt *attachs;
  Achar *data;
};

Value *_value_new(
  enum Value_t t,
  Pos *pos,
  Type *type,
  Avatt *attachs,
  Achar *data
) {
  Value *this = MALLOC(Value);
  this->t = t;
  XNULL(pos)
  this->pos = pos;
  XNULL(type)
  this->type = type;
  XNULL(attachs)
  this->attachs = attachs;
  XNULL(data)
  this->data = data;
  return this;
}

enum Value_t value_t(Value *this) {
  XNULL(this)
  return this->t;
}

Pos *value_pos(Value *this) {
  XNULL(this)
  return this->pos;
}

Type *value_type(Value *this) {
  XNULL(this)
  return this->type;
}

Avatt *value_attachs(Value *this) {
  XNULL(this)
  return this->attachs;
}

Achar *value_data(Value *this) {
  XNULL(this)
  return this->data;
}

Ajson *value_to_json(Value *this) {
  XNULL(this)
  Ajson *serial = ajson_new();
  jarr_aint(serial, this->t);
  ajson_add(serial, json_warray(pos_to_json(this->pos)));
  ajson_add(serial, json_warray(type_to_json(this->type)));
  ajson_add(serial, json_warray(avatt_to_json(this->attachs, vatt_to_json)));
  ajson_add(serial, json_warray(achar_to_json(this->data, str_to_json)));
  return serial;
}

Value *value_from_json(Ajson *serial) {
  XNULL(serial)
  Value *this = MALLOC(Value);
  size_t i = 0;
  this->t = jarr_gint(serial, i++);
  this->pos = pos_from_json(json_rarray(ajson_get(serial, i++)));
  this->type = type_from_json(json_rarray(ajson_get(serial, i++)));
  this->attachs = avatt_from_json(json_rarray(ajson_get(serial, i++)), vatt_from_json);
  this->data = achar_from_json(json_rarray(ajson_get(serial, i++)), str_from_json);
  return this;
}
/*.-.*/

static Achar *mk_achar(char *value) {
  Achar *r = achar_new();
  achar_add(r, value);
  return r;
}

Value *value_new_null(Pos *pos) {
  XNULL(pos)
  return _value_new(
    value_VNULL, pos, type_new_unknown(), avatt_new(), mk_achar("")
  );
}

Value *value_new_bool(Pos *pos, Avatt *atts, char *value) {
  XNULL(pos)
  XNULL(atts)
  XNULL(value)
  return _value_new(
    value_VBOOL, pos, type_new_data("Bool", ltype_new()), atts, mk_achar(value)
  );
}

Value *value_new_byte(Pos *pos, char *value) {
  XNULL(pos)
  XNULL(value)
  return _value_new(
    value_VBYTE, pos, type_new_data("Byte", ltype_new()),
    avatt_new(), mk_achar(value)
  );
}

Value *value_new_int(Pos *pos, char *value) {
  XNULL(pos)
  XNULL(value)
  return _value_new(
    value_VINT, pos, type_new_data("Int", ltype_new()),
    avatt_new(), mk_achar(value)
  );
}

Value *value_new_float(Pos *pos, char *value) {
  XNULL(pos)
  XNULL(value)
  return _value_new(
    value_VFLOAT, pos, type_new_data("Float", ltype_new()),
    avatt_new(), mk_achar(value)
  );
}

Value *value_new_char(Pos *pos, Avatt *atts, char *value) {
  XNULL(pos)
  XNULL(atts)
  XNULL(value)
  return _value_new(
    value_VCHAR, pos, type_new_data("Char", ltype_new()),
    atts, mk_achar(value)
  );
}

Value *value_new_str(Pos *pos, Avatt *atts, char *value) {
  XNULL(pos)
  XNULL(atts)
  XNULL(value)
  return _value_new(
    value_VSTR, pos, type_new_data("Str", ltype_new()),
    atts, mk_achar(value)
  );
}

Value *value_new_str2(Pos *pos, Avatt *atts, char *value) {
  XNULL(pos)
  XNULL(atts)
  XNULL(value)
  return _value_new(
    value_VSTR2, pos, type_new_data("Str", ltype_new()),
    atts, mk_achar(value)
  );
}

static Type *common_type(Ltype *types) {
  Type *r = ltype_head(types);
  EACH(ltype_tail(types), Type, t) {
    if (!type_eq(t, r)) {
      return type_new_unknown();
    }
  }_EACH
  return r;
}

static Ltype *ct_value(Avalue *vs) {
  Type *t;
  if (!avalue_size(vs)) {
    t = type_new_unknown();
  } else {
    Ltype *ts = ltype_new();
    EACH(vs, Value, v) {
      ts = ltype_cons(ts, v->type);
    }_EACH
    t = common_type(ts);
  }
  Ltype *r = ltype_new();
  return ltype_cons(r, t);
}

Value *value_new_arr(Pos *pos, Avatt *atts, Avalue *values) {
  XNULL(pos)
  XNULL(atts)
  XNULL(values)
  return _value_new(
    value_VARR, pos, type_new_data("Arr", ct_value(values)), atts,
    (Achar *)avalue_to_json(values, value_to_json)
  );
}

static Ltype *ct_value2(Avalue *vs) {
  Type *t;
  if (!avalue_size(vs)) {
    t = type_new_unknown();
  } else {
    Ltype *ts = ltype_new();
    bool is_key = true;
    EACH(vs, Value, v) {
      if (is_key) {
        is_key = false;
      } else {
        ts = ltype_cons(ts, v->type);
        is_key = true;
      }
    }_EACH
    t = common_type(ts);
  }
  Ltype *r = ltype_new();
  return ltype_cons(r, t);
}

Value *value_new_map(Pos *pos, Avatt *atts, Avalue *values) {
  XNULL(pos)
  XNULL(atts)
  XNULL(values)
  return _value_new(
    value_VMAP, pos, type_new_data("Map", ct_value2(values)), atts,
    (Achar *)avalue_to_json(values, value_to_json)
  );
}

Value *value_new_fn(Pos *pos, Achar *params, Astat *stats) {
  XNULL(pos)
  XNULL(params)
  XNULL(stats)
  Ajson *js = ajson_new();
  ajson_add(js, json_warray(achar_to_json(params, str_to_json)));
  ajson_add(js, json_warray(astat_to_json(stats, stat_to_json)));
  return _value_new(
    value_VFN, pos, type_new_unknown(), avatt_new(), (Achar *)js
  );
}

Value *value_new_id(Pos *pos, Avatt *atts, char *id, Achar *generics) {
  XNULL(pos)
  XNULL(atts)
  XNULL(id)
  XNULL(generics)
  achar_add(generics, id);
  return _value_new(value_VID, pos, type_new_unknown(), atts, generics);
}

Value *value_new_group(Pos *pos, Avatt *atts, Value *v) {
  XNULL(pos)
  XNULL(atts)
  XNULL(v)
  return _value_new(
    value_VGROUP, pos, v->type, atts, (Achar *)value_to_json(v)
  );
}

Value *value_new_cast(Pos *pos, Type *tp, Value *v) {
  XNULL(pos)
  XNULL(tp)
  XNULL(v)
  return _value_new(
    value_VCAST, pos, tp, avatt_new(), (Achar *)value_to_json(v));
}

Value *value_new_lunary(Pos *pos, char *op, Value *v) {
  XNULL(pos)
  XNULL(op)
  XNULL(v)
  Achar *data = (Achar *)value_to_json(v);
  achar_add(data, op);
  return _value_new(
    value_VLUNARY, pos, value_type(v), avatt_new(), data
  );
}

Value *value_new_runary(Pos *pos, char *op, Value *v) {
  XNULL(pos)
  XNULL(op)
  XNULL(v)
  Achar *data = (Achar *)value_to_json(v);
  achar_add(data, op);
  return _value_new(
    value_VRUNARY, pos, value_type(v), avatt_new(), data
  );
}

Value *value_new_binary(
  Pos *pos, Type *tp, char *op, Value *v1, Value *v2
) {
  XNULL(pos)
  XNULL(tp)
  XNULL(op)
  XNULL(v1)
  XNULL(v2)
  Avalue *vs = avalue_new();
  avalue_add(vs, v1);
  avalue_add(vs, v2);
  Achar *data = (Achar *)avalue_to_json(vs, value_to_json);
  achar_add(data, op);
  return _value_new(value_VBINARY, pos, tp, avatt_new(), data);
}

Value *value_new_ternary(
  Pos *pos, Type *tp, Value *v1, Value *v2, Value *v3
) {
  XNULL(pos)
  XNULL(tp)
  XNULL(v1)
  XNULL(v2)
  XNULL(v3)
  Avalue *vs = avalue_new();
  avalue_add(vs, v1);
  avalue_add(vs, v2);
  avalue_add(vs, v3);
  return _value_new(
    value_VTERNARY, pos, tp, avatt_new(),
    (Achar *)avalue_to_json(vs, value_to_json)
  );
}

Value *value_set_type(Value *this, Type *t) {
  XNULL(this)
  XNULL(t)
  return _value_new(this->t, this->pos, t, this->attachs, this->data);
}
