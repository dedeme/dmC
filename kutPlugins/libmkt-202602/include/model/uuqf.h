// Copyright 08-Feb-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Model references.

#ifndef MODEL_UUQF_H
  #define MODEL_UUQF_H

#include "exp.h"
#include "iface.h"

/// Returns model referencies (Arr<double>)
///   md: Parameters.
Arr *libmkt_uuqf_refs_c (ModelParams *md);

/// Returns model referemces (Exp<[[f.].]>).
///   params: Arr<<cquotes>, [f.]>. Fields are:
///             cqts  : Market quotes.
///             Params: Model parameters.
Exp *libmkt_uuqf_refs (Arr *params);

#endif
