// Copyright 24-Aug-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Array&lt;Type>

#ifndef TYPES_ATYPE_H
  # define TYPES_ATYPE_H

typedef struct type_Type Type;
typedef struct lerror_Lerror L_error;

#define TY Type                    // Element type
#define FN type                    // Function prefix
#include "dmc/tpl/tarr.h"
#undef TY
#undef FN

#endif
