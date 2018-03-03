// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Follow with Weighted Moving Average family

#ifndef FOLLOWWMA_H
  # define FOLLOWWMA_H

#include <dmc/Json.h>
#include "Flea.h"

///
Flea *followwma_new(Flea *f);

///
Flea *followwma_restore(Flea *f, Json *serial);

#endif
