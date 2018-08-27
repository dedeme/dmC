// Copyright 11-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Statement

#ifndef AST_STAT_H
  # define AST_STAT_H

/// Statement types
enum Stat_t {
  stat_VAL,
  stat_VAR,
  stat_ASSIGN,
  stat_FN,
  stat_RETURN,
  stat_BREAK,
  stat_CONTINUE,
  stat_BLOCK,
  stat_WHILE,
  stat_FOR,
  stat_IF,
  stat_TRY,
  stat_THROW,
  stat_NATIVE
};

typedef struct pos_Pos Pos;
typedef struct value_Value Value;
typedef struct aastat_Aastat Aastat;
typedef struct avalue_Avalue Avalue;

/*.-.*/

#include "dmc/Json.h"
#include "dmc/ct/Ajson.h"

///
typedef struct stat_Stat Stat;

///
enum Stat_t stat_t(Stat *this);

///
Pos *stat_pos(Stat *this);

///
char *stat_id(Stat *this);

///
Avalue *stat_values(Stat *this);

///
Aastat *stat_blocks(Stat *this);

///
Ajson *stat_to_json(Stat *this);

///
Stat *stat_from_json(Ajson *s);

/*.-.*/

/// [SVAL]
Stat *stat_new_val(Pos *pos, char *id, Value *value);

/// [SVAR]
Stat *stat_new_var(Pos *pos, char *id, Value *value);

/// [SASSIGN] 'op' is saved in stat_id
Stat *stat_new_assign(
  Pos *pos, char *op, Value *left_value, Value *right_value
);

/// [SFN]
Stat *stat_new_fn(Pos *pos, Value *value);

/// [SBREAK]
Stat *stat_new_break(Pos *pos);

/// [SCONTINUE]
Stat *stat_new_continue(Pos *pos);

/// [STHROW]
Stat *stat_new_throw(Pos *pos, Value *value);

/// [SRETURN]  Value is saved in 'stat_values()'. 'value' can be NULL and
/// then stat_values() is an empty array.
Stat *stat_new_return(Pos *pos, Value *value);

/// [SBLOCK]
Stat *stat_new_block(Pos *pos, Astat *block);

/// [SWHILE]
Stat *stat_new_while(Pos *pos, Value *condition, Astat *block);

/// [SFOR] for has four parts:
///   1: Astat(0):
///       Initial assignements. Statemments of type SVAR, SAVAL or SASSIGN
///   2: Avalue: Loop conditions.
///   3: Astat (1): Final conditions
///   4: Astat (2): Block
Stat *stat_new_for(Pos *pos, Avalue *values, Aastat *blocks);

/// [SIF] If 'if' has clause 'else' arr_size(blocks) == arr_size(values) + 1
Stat *stat_new_if(Pos *pos, Avalue *values, Aastat *blocks);

/// [STRY] If 'blocks' has three elements, there is a 'finally' block.
Stat *stat_new_try(Pos *pos, char *var, Aastat *blocks);

/// [SNATIVE]
Stat *stat_new_native(Pos *pos, char *text);

#endif
