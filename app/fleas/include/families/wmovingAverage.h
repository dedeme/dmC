// Copyright 22-Jan-2018 ºDeme
// GNU Buyeral Public License - V3 <http://www.gnu.org/licenses/>

/// Weight Moving Average family

#ifndef WMOVING_AVERAGE_H
  # define WMOVING_AVERAGE_H

#include <Json.h>
#include "Flea.h"

///
Flea *wmovingAverage_new(Flea *f);

///
Flea *wmovingAverage_restore(Flea *f, Json *serial);

#endif



