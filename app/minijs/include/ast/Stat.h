// Copyright 25-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Ast statement model

#ifndef AST_STAT_H
  #define AST_STAT_H

#include <dmc/all.h>
#include "DEFS.h"

/*.-.*/

///
typedef struct stat_Stat Stat;

///
Stat *stat_new(enum Stat_t stype);

///
enum Stat_t stat_stype(Stat *this);

///
Json *stat_serialize(Stat *this);

///
Stat *stat_restore(Json *s);

/*.-.*/

///
bool stat_eq(Stat *this, Stat *other);

#endif
