// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// List&lt;Error>

#ifndef TYPES_LERROR_H
  # define TYPES_LERROR_H

#include "types/Error.h"

typedef struct lerror_Lerror Lerror;

#define TY Error                    // Element type
#define FN error                    // Function prefix
#include "dmc/tpl/tlist.h"
#undef TY
#undef FN

Lerror *lerror_add_if(Lerror *this, Error *e);

Lerror *lerror_add_list(Lerror *this, Lerror *es);

#endif
