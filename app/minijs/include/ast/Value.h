// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Right value

#ifndef AST_VALUE_H
  # define AST_VALUE_H

#include <dmc/all.h>
#include "ast/Type.h"
#include "Pos.h"
#include "Achar.h"
#include "Avatt.h"

/// Value types
enum Value_t {
  VNULL,
  VBOOL,
  VBYTE,
  VINT,
  VFLOAT,
  VCHAR,
  VSTR,
  VSTR2,
  VARR,
  VMAP,
  VFN,
  VID,
  VGROUP,
  VCAST,
  VNEW,
  VLUNARY,
  VRUNARY,
  VBINARY,
  VTERNARY
};

typedef Arr Avalue;

typedef Arr Astat;

/*.-.*/

///
typedef struct value_Value Value;

///
Value *value_new(
  enum Value_t vtype,
  Pos *pos,
  Type *type,
  Avatt *attachs,
  Achar *data
);

///
enum Value_t value_vtype(Value *this);

///
Pos *value_pos(Value *this);

///
Type *value_type(Value *this);

///
Avatt *value_attachs(Value *this);

///
Achar *value_data(Value *this);

///
Arr/*Json*/ *value_serialize(Value *this);

///
Value *value_restore(Arr/*Json*/ *s);

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

/// kvs are serialized in value_data()
Value *value_new_map(Pos *pos, Avatt *atts, Map/*Value*/ *m);

/// stats are serialized in value_data()
Value *value_new_fn(Pos *pos, Achar *params, Astat *stats);

/// Id must be a valid identifier
Value *value_new_id(Pos *pos, Avatt *atts, char *id, Achar *generics);

/// Values are serialized in value_data()
Value *value_new_new(Pos *pos, Type *tp, Avalue *values);

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
Value *value_new_ternary(Pos *pos, Type *tp, Value *v1, Value *v2, Value *v3);

#endif

