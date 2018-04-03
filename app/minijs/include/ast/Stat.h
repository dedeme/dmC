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
void *stat_data(Stat *this);

/*.-.*/

///
Stat *stat_new_val(Pos *pos, Dvalue *value);

///
Stat *stat_new_var(Pos *pos, Dvalue *value);

///
Stat *stat_new_fn(Pos *pos, Arr/*Value*/ *values);

///
Stat *stat_new_return(Pos *pos, Value *value);

///
Json *stat_serialize(Stat *this);

///
Stat *stat_restore(Json *s);

#endif
