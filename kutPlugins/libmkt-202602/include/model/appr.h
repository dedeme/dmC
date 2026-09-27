// Copyright 07-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Model references.

#ifndef MODEL_APPR_H
  #define MODEL_APPR_H

#include "exp.h"
#include "iface.h"

/// Returns model referencies (Arr<double>)
///   md: Parameters.
Arr *libmkt_appr_refs_c (ModelParams *md);

/// Returns model references (Exp<[[f.].]>).
///   params: Arr<<cquotes>, [f.]>. Fields are:
///             cqts  : Market quotes.
///             Params: Model parameters.
Exp *libmkt_appr_refs (Arr *params);

#endif
