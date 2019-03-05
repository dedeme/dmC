// Copyright 25-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#ifndef CALCASSETS_H
  #define CALCASSETS_H

#include "dmc/std.h"
#include "Flea.h"
#include "Model.h"
#include "Rs.h"

/// Returns a result with the field assets set.
Rs *calcAssets_new(Model *fmodel, Flea *f);

#endif
