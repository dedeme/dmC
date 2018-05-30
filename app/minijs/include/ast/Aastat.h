// Copyright 30-Apr-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr[Astat]

#ifndef AST_AASTAT_H
  # define AST_AASTAT_H

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
Arr/*Json*/ *aastat_serialize(Aastat *this);

///
Aastat *aastat_restore(Arr/*Json*/ *j);


#endif

