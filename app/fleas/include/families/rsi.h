// Copyright 15-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// R.S.I. family

#ifndef RSI_H
  # define RSI_H

#include <dmc/Json.h>
#include "Flea.h"

///
Flea *rsi_new(Flea *f);

///
Flea *rsi_restore(Flea *f, Json *serial);

#endif




