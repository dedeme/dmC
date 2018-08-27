// Copyright 13-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// List of Value *

#ifndef CT_LVALUE_H
  #define CT_LVALUE_H

typedef struct value_Value Value;

#define TY Value
#define FN value
#include "dmc/tpl/tlist.h"
#undef TY
#undef FN

#endif
