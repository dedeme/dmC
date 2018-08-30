// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Arr&lt;Error>

#ifndef TYPES_AFAIL_H
  # define TYPES_AFAIL_H

#include "types/Fail.h"

typedef struct afail_Afail Afail;

#define TY Fail                    // Element type
#define FN fail                    // Function prefix
#include "dmc/tpl/tarr.h"
#undef TY
#undef FN

void afail_add_if(Afail *this, Fail *e);

void afail_add_arr_if(Afail *this, Afail *es);

#endif
