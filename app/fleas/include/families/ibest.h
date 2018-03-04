// Copyright 4-Mar-2018 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Best family of Ibex

#ifndef IBEST_H
  # define IBEST_H

#include <dmc/Json.h>
#include "Flea.h"

///
Flea *ibest_new(Flea *f);

///
Flea *ibest_restore(Flea *f, Json *serial);

#endif

