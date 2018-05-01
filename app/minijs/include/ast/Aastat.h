// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Stat]

#ifndef AST_ASTAT_H
  # define AST_AATT_H

#include "Astat.h"

///
typedef Arr Aastat;

///
Aastat *aastat_new(void);

///
void aastat_add(Aastat *this, Astat *a);

///
Astat *aastat_get(Aastat *this, int ix);

///
Json *aastat_serialize(Aastat *this);

///
Aastat *aastat_restore(Json *j);


#endif

