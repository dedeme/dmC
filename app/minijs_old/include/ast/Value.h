// Copyright 10-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Value in top declarations (static)

#ifndef AST_VALUE_H
  # define AST_VALUE_H

typedef struct pos_Pos Pos;
typedef struct type_Type Type;
typedef struct avatt_Avatt Avatt;
typedef struct avalue_Avalue Avalue;
typedef struct astat_Astat Astat;

/// Value types
enum Value_t {
  value_VNULL,
  value_VBOOL,
  value_VBYTE,
  value_VINT,
  value_VFLOAT,
  value_VCHAR,
  value_VSTR,
  value_VSTR2,
  value_VARR,
  value_VMAP,
  value_VFN,
  value_VID,
  value_VGROUP,
  value_VCAST,
  value_VLUNARY,
  value_VRUNARY,
  value_VBINARY,
  value_VTERNARY
};

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct value_Value Value;

///
enum Value_t value_t(Value *this);

///
Pos *value_pos(Value *this);

///
Type *value_type(Value *this);

///
Avatt *value_attachs(Value *this);

///
Achar *value_data(Value *this);

///
Ajson *value_to_json(Value *this);

///
Value *value_from_json(Ajson *s);

/*.-.*/

///
Value *value_new_null(Pos *pos);

/// Value must be 't' or 'f'
Value *value_new_bool(Pos *pos, Avatt *atts, char *value);

/// Value must be digits
Value *value_new_byte(Pos *pos, char *value);

/// Value must be digits
Value *value_new_int(Pos *pos, char *value);

/// Value must be a floating number
Value *value_new_float(Pos *pos, char *value);

/// Admit escape values kind of \u0023.
Value *value_new_char(Pos *pos, Avatt *atts, char *value);

/// Admit escape values kind of \u0023.
Value *value_new_str(Pos *pos, Avatt *atts, char *value);

/// Admit escape values kind \u0023 and interpolations ${}
Value *value_new_str2(Pos *pos, Avatt *atts, char *value);

/// Values are serialized in value_data()
Value *value_new_arr(Pos *pos, Avatt *atts, Avalue *values);

/// 'values' contains successive key-value pairs
Value *value_new_map(Pos *pos, Avatt *atts, Avalue *values);

/// stats are serialized in value_data()
Value *value_new_fn(Pos *pos, Achar *params, Astat *stats);

/// Id must be a valid identifier
Value *value_new_id(Pos *pos, Avatt *atts, char *id, Achar *generics);

/// Value in parenthesis. v1 is serialized in value_data()
Value *value_new_group(Pos *pos, Avatt *atts, Value *v);

/// v1 is serialized in value_data()
Value *value_new_cast(Pos *pos, Type *tp, Value *v);

///
Value *value_new_lunary(Pos *pos, char *op, Value *v);

///
Value *value_new_runary(Pos *pos, char *op, Value *v);

///
Value *value_new_binary(Pos *pos, Type *tp, char *op, Value *v1, Value *v2);

///
Value *value_new_ternary(
  Pos *pos, Type *tp, Value *v1, Value *v2, Value *v3
);

///
Value *value_set_type(Value *this, Type *t);

#endif
