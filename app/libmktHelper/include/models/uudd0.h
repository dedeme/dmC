// Copyright 29-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// 'up-up/down-down' investor model.

#ifndef MODELS_UUDD0_H
  #define MODELS_UUDD0_H

#include "vec.h"
#include "kut/tp3.h"

/// Returns an Arr<Vec> similar to 'closes' with references.
///   closes: Arr<Vec> where:
///     - Array entries are rows (one for date, from before to after.)
///     - Each row is a vector with columns values (one for company).
///   params: Model params.
Arr *uudd0_calc_references (Arr *closes, Vec *params);

/// Returns Tp3<Vec, int, double> with references, date of new first
/// real reference and its value.
///   date_ix: Firts real reference date index or -1 if it is missing.
///   rref   : Real reference value or -1 if it is missing
///   closes : <Vec> with closes of a company.
///   params : Model params.
Tp3 *uudd0_calc_real_references (
  int date_ix, double rref, Vec *closes, Vec *params
);

#endif
