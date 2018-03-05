// Copyright 4-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Best family of Selected

#ifndef SBEST_H
  # define SBEST_H

#include <dmc/Json.h>
#include "Flea.h"

///
Flea *sbest_new(Flea *f);

///
Flea *sbest_restore(Flea *f, Json *serial);

#endif

