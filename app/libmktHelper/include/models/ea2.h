// Copyright 29-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// 'Exponetial average (2)' investor model.

#ifndef MODELS_EA2_H
  #define MODELS_EA2_H

#include "vec.h"

/// Returns an Arr<Vec> similar to 'closes' with references.
///   closes: Arr<Vec> where:
///     - Array entries are rows (one for date, from before to after.)
///     - Each row is a vector with columns values (one for company).
///   params: Model params.
Arr *ea2_calc_references (Arr *closes, Vec *params);

#endif
