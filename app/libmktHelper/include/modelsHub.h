// Copyright 27-Jan-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Hub of model reference calculators.


#ifndef MODELS_HUB_H
  #define MODELS_HUB_H

#include "kut/map.h"
#include "vec.h"

/// Returns an Arr<Vec> similar to 'closes' with model references.
///   model_id: Model identifier.
///   closes : Arr<Vec> where:
///     - Array entries are rows (one for date, from before to after.)
///     - Each row is a vector with columns values (one for company).
///   params : Model params.
Arr *modelsHub_calc_references (char *model_id, Arr *closes, Vec *params);

/// Returns references for model, parameters and closes.
/// Input:
///   - command (s)     : "mkRefs"
///   - modelId (s)     : Model identifier.
///   - Closes ([[n.].]): Closes matrix of Dates(rows)xCos(columns).
///   - Params ([n.])   : Model parameters.
/// Output:
///   - Refs [[n.].]: References matrix of Dates(rows)xCos(columns).
char *modelsHub_run(Map *rq);

/// Returns real references for model, company, parameters and closes.
/// Input:
///   - command (s)      : "realRefs"
///   - initRf ([[s, n]]): Initial real reference (Option<date(YYYYMMDD), ref>)
///   - modelId (s)      : Model identifier.
///   - Closes ([[n.].]) : Closes matrix of Dates(rows)xCos(columns).
///   - Params ([n.])    : Model parameters.
/// Output:
///   - [Refs, newInitRef] :: [[n.], [s, n]]
///                         : References and new initial real reference.
char *modelsHub_run_real(Map *rq);

#endif
