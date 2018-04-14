// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Ast value model

#ifndef AST_VALUE_H
  #define AST_VALUE_H

#include <dmc/all.h>
#include "types/Type.h"
#include "lexer/Pos.h"
#include "DEFS.h"

/*.-.*/

///
typedef struct value_Value Value;

///
Pos *value_pos(Value *this);

///
enum Vtype_t value_vtype(Value *this);

///
Type *value_type(Value *this);

///
void value_set_type(Value *this, Type *value);

///
Arr *value_ids(Value *this);

///
Arr *value_values(Value *this);

///
Arr *value_attachs(Value *this);

///
Arr *value_stats(Value *this);

///
Json *value_serialize(Value *this);

///
Value *value_restore(Json *s);

/*.-.*/

///
Value *value_new_null(Pos *pos);

/// Value must be 'true' or 'false'
Value *value_new_bool(Pos *pos, Arr/*Attach*/ *atts, char *value);

/// Value must be digits < 256 ending with t (e.g. 124b) + Hexadecimal<br>
/// 'value' only contains digits.
Value *value_new_byte(Pos *pos, Arr/*Attach*/ *atts, char *value);

/// Value must be digits (no control of range)
Value *value_new_int(Pos *pos, char *value);

/// Value must be a number with '.'
Value *value_new_float(Pos *pos, char *value);

/// Value must be between ''. Admit escape values kind \u0023.
Value *value_new_char(Pos *pos, Arr/*Attach*/ *atts, char *value);

/// Value must be between "". Admit escape values kind \u0023.
Value *value_new_str(Pos *pos, Arr/*Attach*/ *atts, char *value);

/// Value must be between ``. Admit escape values kind \u0023.
Value *value_new_str2(Pos *pos, Arr/*Attach*/ *atts, char *value);

///
Value *value_new_arr(Pos *pos, Arr/*Attach*/ *atts, Arr/*Value*/ *values);

/// kvs are pairs of key-value. 'kvs' elements number is pair.
Value *value_new_map(Pos *pos, Arr/*Attach*/ *atts, Arr/*Value*/ *kvs);

/// 
Value *value_new_fn(Pos *pos, Arr/*char*/ *params, Arr/*Stat*/ *stats);

/// Id must be a valid identifier
Value *value_new_id(Pos *pos, Arr/*Attach*/ *atts, char *id);

/// Operator must be one of !, ++, --, +, -
Value *value_new_lunary(Pos *pos, char *operator, Value *value);

/// Operator must be one of ++, --
Value *value_new_runary(Pos *pos, char *operator, Value *value);

/// Operator must be one of +, -, *, /, %, ^, >, <, ==, !=, <=, >=, &&, ||, &,
/// |, ^^, <<, >>, >>>, ?:
Value *value_new_binary(Pos *pos, char *operator, Value *v1, Value *v2);

/// Operator a?x:y
Value *value_new_ternary(Pos *pos, Value *v1, Value *v2, Value *v3);

/// Operator with. Example:
///   val x = with(a) : a+b = 0 : a-b = 1 : _ = 2;
/// The last 'conditions' is value_new_null().
Value *value_new_with(
  Pos *pos, Value *v, Arr/*Value*/ *conditions, Arr/*Value*/ *values
);

/// 'tp' is saved as 'id' removing generics.
Value *value_new_new(Pos *pos, Type *tp, Arr/*Value*/ *values);

/// Value in parenthesis
Value *value_new_group(Pos *pos, Arr/*Attach*/ *atts, Value *v1);

#endif

