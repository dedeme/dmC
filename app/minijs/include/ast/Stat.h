// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Ast statement model

#ifndef AST_STAT_H
  #define AST_STAT_H

#include <dmc/all.h>
#include "DEFS.h"
#include "ast/Dvalue.h"

/*.-.*/

///
typedef struct stat_Stat Stat;

///
Pos *stat_pos(Stat *this);

///
enum Stat_t stat_stype(Stat *this);

///
Arr *stat_ids(Stat *this);

///
Arr *stat_values(Stat *this);

///
Arr *stat_dvalues(Stat *this);

///
Arr *stat_blocks(Stat *this);

///
Json *stat_serialize(Stat *this);

///
Stat *stat_restore(Json *s);

/*.-.*/

/// [SVAL] Value is saved in 'stat_dvalues()'
Stat *stat_new_val(Pos *pos, Dvalue *value);

/// [SVAL] Value is saved in 'stat_dvalues()'
Stat *stat_new_var(Pos *pos, Dvalue *value);

/// [SFN] Value is a function call. It is saved in 'stat_values()'
Stat *stat_new_fn(Pos *pos, Value *value);

/// [SASSIGN] left_value and right_value are saved in 'stat_values()', in
/// [0] 'left' and in [1] 'right'
Stat *stat_new_assign(Pos *pos, Value *left_value, Value *right_value);

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
Stat *stat_new_block(Pos *pos, Arr/*Stat*/ *block);

/// [SWHILE]
Stat *stat_new_while(Pos *pos, Value *condition, Arr/*Stat*/ *block);

/// [SDO]
Stat *stat_new_do(Pos *pos, Value *condition, Arr/*Stat*/ *block);

/// [SFOR0]
Stat *stat_new_for0(Pos *pos, Arr/*Value*/ *values, Arr/*Stat*/ *block);

/// [SFOR]
Stat *stat_new_for(Pos *pos, Arr/*Value*/ *values, Arr/*Stat*/ *block);

/// [SFOR_EACH]
Stat *stat_new_for_each(Pos *pos, Value *value, Arr/*Stat*/ *block);

/// [SIF] If 'if' has clause 'else' arr_size(blocks) == arr_size(values) + 1
Stat *stat_new_if(Pos *pos, Arr/*Value*/ *values, Arr/*Arr[Stat]*/ *blocks);

/// [STRY] If 'blocks' has three elements, there is a 'finally' block.
Stat *stat_new_try(Pos *pos, char *var, Arr/*Arr[Stat]*/ *blocks);

/// [SNATIVE]
Stat *stat_new_native(Pos *pos, char *text);

#endif
