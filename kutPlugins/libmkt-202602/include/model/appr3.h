// Copyright 07-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Model references.

#ifndef MODEL_APPR3_H
  #define MODEL_APPR3_H

#include "exp.h"
#include "iface.h"

/// Returns model referencies (Arr<double>)
///   md: Parameters.
Arr *libmkt_appr3_refs_c (ModelParams *md);

/// Returns model referemces (Exp<[[f.].]>).
///   params: Arr<<cquotes>, [f.]>. Fields are:
///             cqts  : Market quotes.
///             Params: Model parameters.
Exp *libmkt_appr3_refs (Arr *params);

#endif
