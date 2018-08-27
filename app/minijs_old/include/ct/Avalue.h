// Copyright 10-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Array of Value *

#ifndef CT_AVALUE_H
  #define CT_AVALUE_H

typedef struct value_Value Value;

#define TY Value
#define FN value
#include "dmc/tpl/tarr.h"
#undef TY
#undef FN

#endif
