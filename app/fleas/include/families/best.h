// Copyright 4-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Best family

#ifndef BEST_H
  # define BEST_H

#include <dmc/Json.h>
#include "Flea.h"

///
Flea *best_new(Flea *f);

///
Flea *best_restore(Flea *f, Json *serial);

#endif

