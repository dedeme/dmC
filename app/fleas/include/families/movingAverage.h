// Copyright 17-Feb-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// MovingAverage family

#ifndef MOVING_AVERAGE_H
  # define MOVING_AVERAGE_H

#include <dm/Json.h>
#include "Flea.h"

///
Flea *movingAverage_new(Flea *f);

///
Flea *movingAverage_restore(Flea *f, Json *serial);

#endif



