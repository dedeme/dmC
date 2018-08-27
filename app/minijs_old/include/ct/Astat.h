// Copyright 10-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Array of Stat *

#ifndef CT_ASTAT_H
  #define CT_ASTAT_H

typedef struct stat_Stat Stat;

#define TY Stat
#define FN stat
#include "dmc/tpl/tarr.h"
#undef TY
#undef FN

///
Ajson *astat2_to_json(Astat *this);

///
Astat *astat2_from_json(Ajson *js);

#endif
