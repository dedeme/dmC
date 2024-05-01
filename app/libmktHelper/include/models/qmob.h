// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// 'Mobile quantum' investor model.

#ifndef MODELS_QMOB_H
  #define MODELS_QMOB_H

#include "vec.h"

/// Returns an Arr<Vec> similar to 'closes' with references.
///   closes: Arr<Vec> where:
///     - Array entries are rows (one for date, from before to after.)
///     - Each row is a vector with columns values (one for company).
///   params: Model params.
Arr *qmob_calc_references (Arr *closes, Vec *params);

#endif
