// Copyright 29-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Statement

#ifndef AST_STAT_H
  # define AST_STAT_H

#include "Pos.h"
#include "Value.h"

/// Statement types
enum Stat_t {
  SVAL,
  SVAR,
  SASSIGN,
  SFN,
  SRETURN,
  SBREAK,
  SCONTINUE,
  SBLOCK,
  SWHILE,
  SFOR,
  SIF,
  STRY,
  STHROW,
  SNATIVE
};

typedef Arr Avalue;
typedef Arr Aastat;

/*.-.*/

///
typedef struct stat_Stat Stat;

///
Stat *stat_new(
  enum Stat_t type,
  Pos *pos,
  char *id,
  Avalue *values,
  Aastat *blocks
);

///
enum Stat_t stat_type(Stat *this);

///
Pos *stat_pos(Stat *this);

///
char *stat_id(Stat *this);

///
Avalue *stat_values(Stat *this);

///
Aastat *stat_blocks(Stat *this);

///
Arr/*Json*/ *stat_serialize(Stat *this);

///
Stat *stat_restore(Arr/*Json*/ *s);

/*.-.*/

#endif

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
