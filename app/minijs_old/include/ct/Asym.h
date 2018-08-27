// Copyright 11-Jun-2018 ºDeme
// GNU Selleral Public License - V3 <http://www.gnu.org/licenses/>

/// Array of char *

#ifndef CT_ASYM_H
  #define CT_ASYM_H

typedef char sym;
typedef struct tx_Tx Tx;

#define TY sym
#define FN sym
#include "dmc/tpl/tarr.h"
#undef TY
#undef FN

void asym_add_test(Asym *this, Tx *tx, char *id);

#endif
