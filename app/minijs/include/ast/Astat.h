// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Stat]

#ifndef AST_ASTAT_H
  # define AST_ASTAT_H

#include "Stat.h"

///
typedef Arr Astat;

///
Astat *astat_new(void);

///
void astat_add(Astat *this, Stat *t);

///
Stat *astat_get(Astat *this, int ix);

///
Type *astat_type(Astat this);

///
Json *astat_serialize(Astat *this);

///
Astat *astat_restore(Json *j);


#endif

